/*
 * copyright (c) 2009 Michael Niedermayer
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

#ifndef AVFORMAT_METADATA_H
#define AVFORMAT_METADATA_H

/**
 * @file
 * internal metadata API header
 * see avformat.h or the public API!
 */


#include "avformat.h"
#include "third_party/ffmpeg/libavutil/dict.h"

typedef struct AVMetadataConv {
    const char *native;
    const char *generic;
} AVMetadataConv;

void ff_metadata_conv(AVDictionary **pm, const AVMetadataConv *d_conv,
                                       const AVMetadataConv *s_conv);
void ff_metadata_conv_ctx(AVFormatContext *ctx, const AVMetadataConv *d_conv,
                                                const AVMetadataConv *s_conv);

#endif /* AVFORMAT_METADATA_H */
