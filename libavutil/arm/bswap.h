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

#ifndef AVUTIL_ARM_BSWAP_H
#define AVUTIL_ARM_BSWAP_H

#include <stdint.h>
#include "third_party/ffmpeg/config.h"
#include "third_party/ffmpeg/libavutil/attributes.h"

#ifdef __ARMCC_VERSION

#if HAVE_ARMV6
#define av_bswap32 av_bswap32
static av_always_inline av_const uint32_t av_bswap32(uint32_t x)
{
    return __rev(x);
}
#endif /* HAVE_ARMV6 */

#elif HAVE_INLINE_ASM

#if HAVE_ARMV6_INLINE
#define av_bswap16 av_bswap16
static av_always_inline av_const unsigned av_bswap16(unsigned x)
{
    unsigned y;

    __asm__("rev16 %0, %1" : "=r"(y) : "r"(x));
    return y;
}
#endif

#if AV_GCC_VERSION_AT_MOST(4,4)
#define av_bswap32 av_bswap32
static av_always_inline av_const uint32_t av_bswap32(uint32_t x)
{
    uint32_t y;
#if HAVE_ARMV6_INLINE
    __asm__("rev %0, %1" : "=r"(y) : "r"(x));
#else
    uint32_t t;
    __asm__ ("eor %1, %2, %2, ror #16 \n\t"
             "bic %1, %1, #0xFF0000   \n\t"
             "mov %0, %2, ror #8      \n\t"
             "eor %0, %0, %1, lsr #8  \n\t"
             : "=r"(y), "=&r"(t) : "r"(x));
#endif /* HAVE_ARMV6_INLINE */
    return y;
}
#endif /* AV_GCC_VERSION_AT_MOST(4,4) */

#endif /* __ARMCC_VERSION */

#endif /* AVUTIL_ARM_BSWAP_H */
