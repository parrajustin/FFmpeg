/*
 * Copyright (c) 2002 Michael Niedermayer <michaelni@gmx.at>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file
 * swap UV filter
 */

#include "third_party/ffmpeg/libavutil/opt.h"
#include "third_party/ffmpeg/libavutil/pixdesc.h"
#include "avfilter.h"
#include "formats.h"
#include "internal.h"
#include "video.h"

typedef struct SwapUVContext {
    const AVClass *class;
} SwapUVContext;

static const AVOption swapuv_options[] = {
    { NULL }
};

AVFILTER_DEFINE_CLASS(swapuv);

static void do_swap(AVFrame *frame)
{
    FFSWAP(uint8_t*,     frame->data[1],     frame->data[2]);
    FFSWAP(int,          frame->linesize[1], frame->linesize[2]);
    FFSWAP(AVBufferRef*, frame->buf[1],      frame->buf[2]);
}

static AVFrame *get_video_buffer(AVFilterLink *link, int w, int h)
{
    AVFrame *picref = ff_default_get_video_buffer(link, w, h);
    do_swap(picref);
    return picref;
}

static int filter_frame(AVFilterLink *link, AVFrame *inpicref)
{
    do_swap(inpicref);
    return ff_filter_frame(link->dst->outputs[0], inpicref);
}

static int is_planar_yuv(const AVPixFmtDescriptor *desc)
{
    int i;

    if (desc->flags & ~(AV_PIX_FMT_FLAG_BE | AV_PIX_FMT_FLAG_PLANAR | AV_PIX_FMT_FLAG_ALPHA) ||
        desc->nb_components < 3 ||
        (desc->comp[1].depth != desc->comp[2].depth))
        return 0;
    for (i = 0; i < desc->nb_components; i++) {
        if (desc->comp[i].offset != 0 ||
            desc->comp[i].shift != 0 ||
            desc->comp[i].plane != i)
            return 0;
    }

    return 1;
}

static int query_formats(AVFilterContext *ctx)
{
    AVFilterFormats *formats = NULL;
    int fmt, ret;

    for (fmt = 0; av_pix_fmt_desc_get(fmt); fmt++) {
        const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get(fmt);
        if (is_planar_yuv(desc) && (ret = ff_add_format(&formats, fmt)) < 0)
            return ret;
    }

    return ff_set_common_formats(ctx, formats);
}

static const AVFilterPad swapuv_inputs[] = {
    {
        .name             = "default",
        .type             = AVMEDIA_TYPE_VIDEO,
        .get_buffer.video = get_video_buffer,
        .filter_frame     = filter_frame,
    },
};

static const AVFilterPad swapuv_outputs[] = {
    {
        .name = "default",
        .type = AVMEDIA_TYPE_VIDEO,
    },
};

const AVFilter ff_vf_swapuv = {
    .name          = "swapuv",
    .description   = NULL_IF_CONFIG_SMALL("Swap U and V components."),
    .priv_size     = sizeof(SwapUVContext),
    .priv_class    = &swapuv_class,
    FILTER_INPUTS(swapuv_inputs),
    FILTER_OUTPUTS(swapuv_outputs),
    FILTER_QUERY_FUNC(query_formats),
    .flags         = AVFILTER_FLAG_SUPPORT_TIMELINE_GENERIC,
};
