/*
 * Copyright (c) 2020 Loongson Technology Corporation Limited
 * Contributed by Shiyou Yin <yinshiyou-hf@loongson.cn>
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

#ifndef AVCODEC_LOONGARCH_H264QPEL_LASX_H
#define AVCODEC_LOONGARCH_H264QPEL_LASX_H

#include <stdint.h>
#include <stddef.h>
#include "third_party/ffmpeg/libavcodec/h264.h"

void ff_h264_h_lpf_luma_inter_lasx(uint8_t *src, int stride,
                                   int alpha, int beta, int8_t *tc0);
void ff_h264_v_lpf_luma_inter_lasx(uint8_t *src, int stride,
                                   int alpha, int beta, int8_t *tc0);
void ff_put_h264_qpel16_mc00_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_put_h264_qpel16_mc10_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_put_h264_qpel16_mc20_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_put_h264_qpel16_mc30_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_put_h264_qpel16_mc01_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_put_h264_qpel16_mc11_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_put_h264_qpel16_mc21_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_put_h264_qpel16_mc31_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_put_h264_qpel16_mc02_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_put_h264_qpel16_mc12_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_put_h264_qpel16_mc32_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_put_h264_qpel16_mc22_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_put_h264_qpel16_mc03_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_put_h264_qpel16_mc13_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_put_h264_qpel16_mc23_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_put_h264_qpel16_mc33_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_avg_h264_qpel16_mc00_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_avg_h264_qpel16_mc10_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_avg_h264_qpel16_mc20_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_avg_h264_qpel16_mc30_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_avg_h264_qpel16_mc01_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_avg_h264_qpel16_mc11_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_avg_h264_qpel16_mc21_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_avg_h264_qpel16_mc31_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_avg_h264_qpel16_mc02_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_avg_h264_qpel16_mc12_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_avg_h264_qpel16_mc22_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_avg_h264_qpel16_mc32_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_avg_h264_qpel16_mc03_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_avg_h264_qpel16_mc13_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_avg_h264_qpel16_mc23_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);
void ff_avg_h264_qpel16_mc33_lasx(uint8_t *dst, const uint8_t *src,
                                  ptrdiff_t dst_stride);

void ff_put_h264_qpel8_mc00_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t stride);
void ff_put_h264_qpel8_mc10_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t stride);
void ff_put_h264_qpel8_mc20_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t stride);
void ff_put_h264_qpel8_mc30_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t stride);
void ff_put_h264_qpel8_mc01_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t stride);
void ff_put_h264_qpel8_mc11_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t stride);
void ff_put_h264_qpel8_mc21_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t stride);
void ff_put_h264_qpel8_mc31_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t stride);
void ff_put_h264_qpel8_mc02_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t stride);
void ff_put_h264_qpel8_mc12_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t stride);
void ff_put_h264_qpel8_mc22_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t stride);
void ff_put_h264_qpel8_mc32_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t stride);
void ff_put_h264_qpel8_mc03_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t stride);
void ff_put_h264_qpel8_mc13_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t stride);
void ff_put_h264_qpel8_mc23_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t stride);
void ff_put_h264_qpel8_mc33_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t stride);
void ff_avg_h264_qpel8_mc00_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t dst_stride);
void ff_avg_h264_qpel8_mc10_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t dst_stride);
void ff_avg_h264_qpel8_mc20_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t dst_stride);
void ff_avg_h264_qpel8_mc30_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t dst_stride);
void ff_avg_h264_qpel8_mc11_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t dst_stride);
void ff_avg_h264_qpel8_mc21_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t dst_stride);
void ff_avg_h264_qpel8_mc31_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t dst_stride);
void ff_avg_h264_qpel8_mc02_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t dst_stride);
void ff_avg_h264_qpel8_mc12_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t dst_stride);
void ff_avg_h264_qpel8_mc22_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t dst_stride);
void ff_avg_h264_qpel8_mc32_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t dst_stride);
void ff_avg_h264_qpel8_mc13_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t dst_stride);
void ff_avg_h264_qpel8_mc23_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t dst_stride);
void ff_avg_h264_qpel8_mc33_lasx(uint8_t *dst, const uint8_t *src,
                                 ptrdiff_t dst_stride);
#endif  // #ifndef AVCODEC_LOONGARCH_H264QPEL_LASX_H
