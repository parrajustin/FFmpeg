/*
 * Copyright (c) 2008 Alexander Strange <astrange@ithinksw.com>
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

/**
 * @file
 * Multithreading support functions
 * @author Alexander Strange <astrange@ithinksw.com>
 */

#ifndef AVCODEC_THREAD_H
#define AVCODEC_THREAD_H

#include "third_party/ffmpeg/libavutil/buffer.h"

#include "avcodec.h"

/**
 * Wait for decoding threads to finish and reset internal state.
 * Called by avcodec_flush_buffers().
 *
 * @param avctx The context.
 */
void ff_thread_flush(AVCodecContext *avctx);

/**
 * Submit a new frame to a decoding thread.
 * Returns the next available frame in picture. *got_picture_ptr
 * will be 0 if none is available.
 * The return value on success is the size of the consumed packet for
 * compatibility with FFCodec.decode. This means the decoder
 * has to consume the full packet.
 *
 * Parameters are the same as FFCodec.decode.
 */
int ff_thread_decode_frame(AVCodecContext *avctx, AVFrame *picture,
                           int *got_picture_ptr, AVPacket *avpkt);

/**
 * If the codec defines update_thread_context(), call this
 * when they are ready for the next thread to start decoding
 * the next frame. After calling it, do not change any variables
 * read by the update_thread_context() method, or call ff_thread_get_buffer().
 *
 * @param avctx The context.
 */
void ff_thread_finish_setup(AVCodecContext *avctx);

#define ff_thread_get_format ff_get_format

/**
 * Wrapper around get_buffer() for frame-multithreaded codecs.
 * Call this function instead of ff_get_buffer(f).
 * Cannot be called after the codec has called ff_thread_finish_setup().
 *
 * @param avctx The current context.
 * @param f The frame to write into.
 */
int ff_thread_get_buffer(AVCodecContext *avctx, AVFrame *f, int flags);

/**
 * Wrapper around release_buffer() frame-for multithreaded codecs.
 * Call this function instead of avctx->release_buffer(f).
 * The AVFrame will be copied and the actual release_buffer() call
 * will be performed later. The contents of data pointed to by the
 * AVFrame should not be changed until ff_thread_get_buffer() is called
 * on it.
 *
 * @param avctx The current context.
 * @param f The picture being released.
 */
void ff_thread_release_buffer(AVCodecContext *avctx, AVFrame *f);

int ff_thread_init(AVCodecContext *s);
int ff_slice_thread_execute_with_mainfunc(AVCodecContext *avctx,
        int (*action_func2)(AVCodecContext *c, void *arg, int jobnr, int threadnr),
        int (*main_func)(AVCodecContext *c), void *arg, int *ret, int job_count);
void ff_thread_free(AVCodecContext *s);
int ff_slice_thread_allocz_entries(AVCodecContext *avctx, int count);
int ff_slice_thread_init_progress(AVCodecContext *avctx);
void ff_thread_report_progress2(AVCodecContext *avctx, int field, int thread, int n);
void ff_thread_await_progress2(AVCodecContext *avctx,  int field, int thread, int shift);

#endif /* AVCODEC_THREAD_H */
