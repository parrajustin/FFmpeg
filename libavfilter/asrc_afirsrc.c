/*
 * Copyright (c) 2020 Paul B Mahol
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with FFmpeg; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "third_party/ffmpeg/libavutil/channel_layout.h"
#include "third_party/ffmpeg/libavutil/eval.h"
#include "third_party/ffmpeg/libavutil/opt.h"
#include "third_party/ffmpeg/libavutil/tx.h"
#include "audio.h"
#include "avfilter.h"
#include "filters.h"
#include "internal.h"
#include "window_func.h"

typedef struct AudioFIRSourceContext {
    const AVClass *class;

    char *freq_points_str;
    char *magnitude_str;
    char *phase_str;
    int nb_taps;
    int sample_rate;
    int nb_samples;
    int win_func;

    AVComplexFloat *complexf;
    float *freq;
    float *magnitude;
    float *phase;
    int freq_size;
    int magnitude_size;
    int phase_size;
    int nb_freq;
    int nb_magnitude;
    int nb_phase;

    float *taps;
    float *win;
    int64_t pts;

    AVTXContext *tx_ctx;
    av_tx_fn tx_fn;
} AudioFIRSourceContext;

#define OFFSET(x) offsetof(AudioFIRSourceContext, x)
#define FLAGS AV_OPT_FLAG_AUDIO_PARAM|AV_OPT_FLAG_FILTERING_PARAM

static const AVOption afirsrc_options[] = {
    { "taps",      "set number of taps",   OFFSET(nb_taps),         AV_OPT_TYPE_INT,    {.i64=1025}, 9, UINT16_MAX, FLAGS },
    { "t",         "set number of taps",   OFFSET(nb_taps),         AV_OPT_TYPE_INT,    {.i64=1025}, 9, UINT16_MAX, FLAGS },
    { "frequency", "set frequency points", OFFSET(freq_points_str), AV_OPT_TYPE_STRING, {.str="0 1"}, 0, 0, FLAGS },
    { "f",         "set frequency points", OFFSET(freq_points_str), AV_OPT_TYPE_STRING, {.str="0 1"}, 0, 0, FLAGS },
    { "magnitude", "set magnitude values", OFFSET(magnitude_str),   AV_OPT_TYPE_STRING, {.str="1 1"}, 0, 0, FLAGS },
    { "m",         "set magnitude values", OFFSET(magnitude_str),   AV_OPT_TYPE_STRING, {.str="1 1"}, 0, 0, FLAGS },
    { "phase",     "set phase values",     OFFSET(phase_str),       AV_OPT_TYPE_STRING, {.str="0 0"}, 0, 0, FLAGS },
    { "p",         "set phase values",     OFFSET(phase_str),       AV_OPT_TYPE_STRING, {.str="0 0"}, 0, 0, FLAGS },
    { "sample_rate", "set sample rate",    OFFSET(sample_rate), AV_OPT_TYPE_INT, {.i64=44100},  1, INT_MAX,    FLAGS },
    { "r",           "set sample rate",    OFFSET(sample_rate), AV_OPT_TYPE_INT, {.i64=44100},  1, INT_MAX,    FLAGS },
    { "nb_samples", "set the number of samples per requested frame", OFFSET(nb_samples), AV_OPT_TYPE_INT, {.i64 = 1024}, 1, INT_MAX, FLAGS },
    { "n",          "set the number of samples per requested frame", OFFSET(nb_samples), AV_OPT_TYPE_INT, {.i64 = 1024}, 1, INT_MAX, FLAGS },
    WIN_FUNC_OPTION("win_func", OFFSET(win_func), FLAGS, WFUNC_BLACKMAN),
    WIN_FUNC_OPTION("w",        OFFSET(win_func), FLAGS, WFUNC_BLACKMAN),
    {NULL}
};

AVFILTER_DEFINE_CLASS(afirsrc);

static av_cold int init(AVFilterContext *ctx)
{
    AudioFIRSourceContext *s = ctx->priv;

    if (!(s->nb_taps & 1)) {
        av_log(s, AV_LOG_WARNING, "Number of taps %d must be odd length.\n", s->nb_taps);
        s->nb_taps |= 1;
    }

    return 0;
}

static av_cold void uninit(AVFilterContext *ctx)
{
    AudioFIRSourceContext *s = ctx->priv;

    av_freep(&s->win);
    av_freep(&s->taps);
    av_freep(&s->freq);
    av_freep(&s->magnitude);
    av_freep(&s->phase);
    av_freep(&s->complexf);
    av_tx_uninit(&s->tx_ctx);
}

static av_cold int query_formats(AVFilterContext *ctx)
{
    AudioFIRSourceContext *s = ctx->priv;
    static const AVChannelLayout chlayouts[] = { AV_CHANNEL_LAYOUT_MONO, { 0 } };
    int sample_rates[] = { s->sample_rate, -1 };
    static const enum AVSampleFormat sample_fmts[] = {
        AV_SAMPLE_FMT_FLT,
        AV_SAMPLE_FMT_NONE
    };
    int ret = ff_set_common_formats_from_list(ctx, sample_fmts);
    if (ret < 0)
        return ret;

    ret = ff_set_common_channel_layouts_from_list(ctx, chlayouts);
    if (ret < 0)
        return ret;

    return ff_set_common_samplerates_from_list(ctx, sample_rates);
}

static int parse_string(char *str, float **items, int *nb_items, int *items_size)
{
    float *new_items;
    char *tail;

    new_items = av_fast_realloc(NULL, items_size, 1 * sizeof(float));
    if (!new_items)
        return AVERROR(ENOMEM);
    *items = new_items;

    tail = str;
    if (!tail)
        return AVERROR(EINVAL);

    do {
        (*items)[(*nb_items)++] = av_strtod(tail, &tail);
        new_items = av_fast_realloc(*items, items_size, (*nb_items + 1) * sizeof(float));
        if (!new_items)
            return AVERROR(ENOMEM);
        *items = new_items;
        if (tail && *tail)
            tail++;
    } while (tail && *tail);

    return 0;
}

static void lininterp(AVComplexFloat *complexf,
                      const float *freq,
                      const float *magnitude,
                      const float *phase,
                      int m, int minterp)
{
    for (int i = 0; i < minterp; i++) {
        for (int j = 1; j < m; j++) {
            const float x = i / (float)minterp;

            if (x <= freq[j]) {
                const float mg = (x - freq[j-1]) / (freq[j] - freq[j-1]) * (magnitude[j] - magnitude[j-1]) + magnitude[j-1];
                const float ph = (x - freq[j-1]) / (freq[j] - freq[j-1]) * (phase[j] - phase[j-1]) + phase[j-1];

                complexf[i].re = mg * cosf(ph);
                complexf[i].im = mg * sinf(ph);
                break;
            }
        }
    }
}

static av_cold int config_output(AVFilterLink *outlink)
{
    AVFilterContext *ctx = outlink->src;
    AudioFIRSourceContext *s = ctx->priv;
    float overlap, scale = 1.f, compensation;
    int fft_size, middle, ret;

    s->nb_freq = s->nb_magnitude = s->nb_phase = 0;

    ret = parse_string(s->freq_points_str, &s->freq, &s->nb_freq, &s->freq_size);
    if (ret < 0)
        return ret;

    ret = parse_string(s->magnitude_str, &s->magnitude, &s->nb_magnitude, &s->magnitude_size);
    if (ret < 0)
        return ret;

    ret = parse_string(s->phase_str, &s->phase, &s->nb_phase, &s->phase_size);
    if (ret < 0)
        return ret;

    if (s->nb_freq != s->nb_magnitude && s->nb_freq != s->nb_phase && s->nb_freq >= 2) {
        av_log(ctx, AV_LOG_ERROR, "Number of frequencies, magnitudes and phases must be same and >= 2.\n");
        return AVERROR(EINVAL);
    }

    for (int i = 0; i < s->nb_freq; i++) {
        if (i == 0 && s->freq[i] != 0.f) {
            av_log(ctx, AV_LOG_ERROR, "First frequency must be 0.\n");
            return AVERROR(EINVAL);
        }

        if (i == s->nb_freq - 1 && s->freq[i] != 1.f) {
            av_log(ctx, AV_LOG_ERROR, "Last frequency must be 1.\n");
            return AVERROR(EINVAL);
        }

        if (i && s->freq[i] < s->freq[i-1]) {
            av_log(ctx, AV_LOG_ERROR, "Frequencies must be in increasing order.\n");
            return AVERROR(EINVAL);
        }
    }

    fft_size = 1 << (av_log2(s->nb_taps) + 1);
    s->complexf = av_calloc(fft_size * 2, sizeof(*s->complexf));
    if (!s->complexf)
        return AVERROR(ENOMEM);

    ret = av_tx_init(&s->tx_ctx, &s->tx_fn, AV_TX_FLOAT_FFT, 1, fft_size, &scale, 0);
    if (ret < 0)
        return ret;

    s->taps = av_calloc(s->nb_taps, sizeof(*s->taps));
    if (!s->taps)
        return AVERROR(ENOMEM);

    s->win = av_calloc(s->nb_taps, sizeof(*s->win));
    if (!s->win)
        return AVERROR(ENOMEM);

    generate_window_func(s->win, s->nb_taps, s->win_func, &overlap);

    lininterp(s->complexf, s->freq, s->magnitude, s->phase, s->nb_freq, fft_size / 2);

    s->tx_fn(s->tx_ctx, s->complexf + fft_size, s->complexf, sizeof(*s->complexf));

    compensation = 2.f / fft_size;
    middle = s->nb_taps / 2;

    for (int i = 0; i <= middle; i++) {
        s->taps[         i] = s->complexf[fft_size + middle - i].re * compensation * s->win[i];
        s->taps[middle + i] = s->complexf[fft_size          + i].re * compensation * s->win[middle + i];
    }

    s->pts = 0;

    return 0;
}

static int activate(AVFilterContext *ctx)
{
    AVFilterLink *outlink = ctx->outputs[0];
    AudioFIRSourceContext *s = ctx->priv;
    AVFrame *frame;
    int nb_samples;

    if (!ff_outlink_frame_wanted(outlink))
        return FFERROR_NOT_READY;

    nb_samples = FFMIN(s->nb_samples, s->nb_taps - s->pts);
    if (nb_samples <= 0) {
        ff_outlink_set_status(outlink, AVERROR_EOF, s->pts);
        return 0;
    }

    if (!(frame = ff_get_audio_buffer(outlink, nb_samples)))
        return AVERROR(ENOMEM);

    memcpy(frame->data[0], s->taps + s->pts, nb_samples * sizeof(float));

    frame->pts = s->pts;
    s->pts    += nb_samples;
    return ff_filter_frame(outlink, frame);
}

static const AVFilterPad afirsrc_outputs[] = {
    {
        .name          = "default",
        .type          = AVMEDIA_TYPE_AUDIO,
        .config_props  = config_output,
    },
};

const AVFilter ff_asrc_afirsrc = {
    .name          = "afirsrc",
    .description   = NULL_IF_CONFIG_SMALL("Generate a FIR coefficients audio stream."),
    .init          = init,
    .uninit        = uninit,
    .activate      = activate,
    .priv_size     = sizeof(AudioFIRSourceContext),
    .inputs        = NULL,
    FILTER_OUTPUTS(afirsrc_outputs),
    FILTER_QUERY_FUNC(query_formats),
    .priv_class    = &afirsrc_class,
};
