/*
 * Copyright (c) 2015 Manojkumar Bhosale (Manojkumar.Bhosale@imgtec.com)
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

#include "third_party/ffmpeg/libavutil/attributes.h"
#include "third_party/ffmpeg/libavutil/mips/cpu.h"
#include "h263dsp_mips.h"
#include "mpegvideo_mips.h"

av_cold void ff_mpv_common_init_mips(MpegEncContext *s)
{
    int cpu_flags = av_get_cpu_flags();

    if (have_mmi(cpu_flags)) {
        s->dct_unquantize_h263_intra = ff_dct_unquantize_h263_intra_mmi;
        s->dct_unquantize_h263_inter = ff_dct_unquantize_h263_inter_mmi;
        s->dct_unquantize_mpeg1_intra = ff_dct_unquantize_mpeg1_intra_mmi;
        s->dct_unquantize_mpeg1_inter = ff_dct_unquantize_mpeg1_inter_mmi;

        if (!(s->avctx->flags & AV_CODEC_FLAG_BITEXACT))
            if (!s->q_scale_type)
                s->dct_unquantize_mpeg2_intra = ff_dct_unquantize_mpeg2_intra_mmi;

        s->denoise_dct= ff_denoise_dct_mmi;
    }

    if (have_msa(cpu_flags)) {
        s->dct_unquantize_h263_intra = ff_dct_unquantize_h263_intra_msa;
        s->dct_unquantize_h263_inter = ff_dct_unquantize_h263_inter_msa;
        if (!s->q_scale_type)
            s->dct_unquantize_mpeg2_inter = ff_dct_unquantize_mpeg2_inter_msa;
    }
}
