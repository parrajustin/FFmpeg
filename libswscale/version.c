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
#include "third_party/ffmpeg/libswscale/swscale.h"
#include "third_party/ffmpeg/libswscale/version.h"

unsigned swscale_version(void)
{
    av_assert0(LIBSWSCALE_VERSION_MICRO >= 100);
    return LIBSWSCALE_VERSION_INT;
}

const char *swscale_configuration(void)
{
    return FFMPEG_CONFIGURATION;
}

const char *swscale_license(void)
{
#define LICENSE_PREFIX "libswscale license: "
    return &LICENSE_PREFIX FFMPEG_LICENSE[sizeof(LICENSE_PREFIX) - 1];
}
