/*
 * Copyright © 2022 Rémi Denis-Courmont.
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

#include <stdint.h>

#include "third_party/ffmpeg/config.h"
#include "third_party/ffmpeg/libavutil/attributes.h"
#include "third_party/ffmpeg/libavutil/cpu.h"
#include "third_party/ffmpeg/libavcodec/fmtconvert.h"

void ff_int32_to_float_fmul_scalar_rvv(float *dst, const int32_t *src,
                                       float mul, int len);
void ff_int32_to_float_fmul_array8_rvv(FmtConvertContext *c, float *dst,
                                       const int32_t *src, const float *mul,
                                       int len);

av_cold void ff_fmt_convert_init_riscv(FmtConvertContext *c)
{
#if HAVE_RVV
    int flags = av_get_cpu_flags();

    if (flags & AV_CPU_FLAG_RVV_F32) {
        c->int32_to_float_fmul_scalar = ff_int32_to_float_fmul_scalar_rvv;
        c->int32_to_float_fmul_array8 = ff_int32_to_float_fmul_array8_rvv;
    }
#endif
}
