/*
 * Version functions.
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

#include "third_party/ffmpeg/config.h"
#include "third_party/ffmpeg/libavutil/avassert.h"
#include "third_party/ffmpeg/libswresample/swresample.h"
#include "third_party/ffmpeg/libswresample/version.h"

#include "third_party/ffmpeg/libavutil/ffversion.h"
const char swr_ffversion[] = "FFmpeg version " FFMPEG_VERSION;

unsigned swresample_version(void)
{
    av_assert0(LIBSWRESAMPLE_VERSION_MICRO >= 100);
    return LIBSWRESAMPLE_VERSION_INT;
}

const char *swresample_configuration(void)
{
    return FFMPEG_CONFIGURATION;
}

const char *swresample_license(void)
{
#define LICENSE_PREFIX "libswresample license: "
    return &LICENSE_PREFIX FFMPEG_LICENSE[sizeof(LICENSE_PREFIX) - 1];
}

