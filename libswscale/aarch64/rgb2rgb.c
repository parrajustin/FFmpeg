/*
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
#include "third_party/ffmpeg/libavutil/aarch64/cpu.h"
#include "third_party/ffmpeg/libavutil/cpu.h"
#include "third_party/ffmpeg/libavutil/bswap.h"
#include "third_party/ffmpeg/libswscale/rgb2rgb.h"
#include "third_party/ffmpeg/libswscale/swscale.h"
#include "third_party/ffmpeg/libswscale/swscale_internal.h"

void ff_interleave_bytes_neon(const uint8_t *src1, const uint8_t *src2,
                              uint8_t *dest, int width, int height,
                              int src1Stride, int src2Stride, int dstStride);

av_cold void rgb2rgb_init_aarch64(void)
{
    int cpu_flags = av_get_cpu_flags();

    if (have_neon(cpu_flags)) {
        interleaveBytes = ff_interleave_bytes_neon;
    }
}
