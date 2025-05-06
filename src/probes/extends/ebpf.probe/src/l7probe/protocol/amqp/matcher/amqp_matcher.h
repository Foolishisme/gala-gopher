/******************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * gala-gopher licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Author: wangshuyuan
 * Create: 2024-10-08
 * Description: AMQP protocol matcher interface
 ******************************************************************************/
#ifndef __AMQP_MATCHER_H__
#define __AMQP_MATCHER_H__

#include "../../include/data_stream.h"
#include "../model/amqp_msg_format.h"

/**
 * Match request and response frames into a record
 *
 * @param req_frame request frame
 * @param resp_frame response frame
 * @param record_buf output record buffer
 */
void amqp_match_frames(struct frame_buf_s *req_frame, struct frame_buf_s *resp_frame, struct record_buf_s *record_buf);

/**
 * Free AMQP record data
 *
 * @param record AMQP record to be freed
 */
void free_amqp_record(struct amqp_record_s *record);

/**
 * Free AMQP message data
 *
 * @param msg AMQP message to be freed
 */
void free_amqp_msg(struct amqp_message_s *msg);

#endif /* __AMQP_MATCHER_H__ */ 