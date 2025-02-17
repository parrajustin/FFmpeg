/**
 * Copyright (c) 2016 Neil Birkbeck <neil.birkbeck@gmail.com>
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
#include <string.h>

#include "third_party/ffmpeg/libavutil/mastering_display_metadata.h"
#include "third_party/ffmpeg/libavutil/mem.h"

AVMasteringDisplayMetadata *av_mastering_display_metadata_alloc(void)
{
    return av_mallocz(sizeof(AVMasteringDisplayMetadata));
}

AVMasteringDisplayMetadata *av_mastering_display_metadata_create_side_data(AVFrame *frame)
{
    AVFrameSideData *side_data = av_frame_new_side_data(frame,
                                                        AV_FRAME_DATA_MASTERING_DISPLAY_METADATA,
                                                        sizeof(AVMasteringDisplayMetadata));
    if (!side_data)
        return NULL;

    memset(side_data->data, 0, sizeof(AVMasteringDisplayMetadata));

    return (AVMasteringDisplayMetadata *)side_data->data;
}

AVContentLightMetadata *av_content_light_metadata_alloc(size_t *size)
{
    AVContentLightMetadata *metadata = av_mallocz(sizeof(AVContentLightMetadata));

    if (size)
        *size = sizeof(*metadata);

    return metadata;
}

AVContentLightMetadata *av_content_light_metadata_create_side_data(AVFrame *frame)
{
    AVFrameSideData *side_data = av_frame_new_side_data(frame,
                                                        AV_FRAME_DATA_CONTENT_LIGHT_LEVEL,
                                                        sizeof(AVContentLightMetadata));
    if (!side_data)
        return NULL;

    memset(side_data->data, 0, sizeof(AVContentLightMetadata));

    return (AVContentLightMetadata *)side_data->data;
}
