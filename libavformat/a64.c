/*
 * a64 muxer
 * Copyright (c) 2009 Tobias Bindhammer
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

#include "third_party/ffmpeg/libavutil/intreadwrite.h"
#include "third_party/ffmpeg/libavcodec/codec_id.h"
#include "third_party/ffmpeg/libavcodec/codec_par.h"
#include "avformat.h"
#include "mux.h"
#include "rawenc.h"

static int a64_write_header(AVFormatContext *s)
{
    AVCodecParameters *par = s->streams[0]->codecpar;
    uint8_t header[5] = {
        0x00, //load
        0x40, //address
        0x00, //mode
        0x00, //charset_lifetime (multi only)
        0x00  //fps in 50/fps;
    };

    if (par->extradata_size < 4) {
        av_log(s, AV_LOG_ERROR, "Missing extradata\n");
        return AVERROR_INVALIDDATA;
    }

    switch (par->codec_id) {
    case AV_CODEC_ID_A64_MULTI:
        header[2] = 0x00;
        header[3] = AV_RB32(par->extradata+0);
        header[4] = 2;
        break;
    case AV_CODEC_ID_A64_MULTI5:
        header[2] = 0x01;
        header[3] = AV_RB32(par->extradata+0);
        header[4] = 3;
        break;
    default:
        return AVERROR_INVALIDDATA;
    }
    avio_write(s->pb, header, 2);
    return 0;
}

const FFOutputFormat ff_a64_muxer = {
    .p.name         = "a64",
    .p.long_name    = NULL_IF_CONFIG_SMALL("a64 - video for Commodore 64"),
    .p.extensions   = "a64, A64",
    .p.video_codec  = AV_CODEC_ID_A64_MULTI,
    .write_header   = a64_write_header,
    .write_packet   = ff_raw_write_packet,
};
