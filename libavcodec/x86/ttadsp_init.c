/*
 * Copyright (c) 2014 James Almer
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
#include "third_party/ffmpeg/libavcodec/ttadsp.h"
#include "third_party/ffmpeg/libavutil/x86/cpu.h"
#include "third_party/ffmpeg/config.h"

void ff_tta_filter_process_ssse3(int32_t *qm, int32_t *dx, int32_t *dl,
                                 int32_t *error, int32_t *in, int32_t shift,
                                 int32_t round);
void ff_tta_filter_process_sse4(int32_t *qm, int32_t *dx, int32_t *dl,
                                int32_t *error, int32_t *in, int32_t shift,
                                int32_t round);

av_cold void ff_ttadsp_init_x86(TTADSPContext *c)
{
#if HAVE_X86ASM
    int cpu_flags = av_get_cpu_flags();

    if (EXTERNAL_SSSE3(cpu_flags))
        c->filter_process = ff_tta_filter_process_ssse3;
    if (EXTERNAL_SSE4(cpu_flags))
        c->filter_process = ff_tta_filter_process_sse4;
#endif
}
