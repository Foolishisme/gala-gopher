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
 * Description: AMQP protocol parser interface
 ******************************************************************************/
#ifndef __AMQP_PARSER_H__
#define __AMQP_PARSER_H__

#include "../../include/data_stream.h"
#include "../model/amqp_msg_format.h"

/**
 * Parse AMQP frame from raw data
 *
 * @param msg_type message type (request/response)
 * @param raw_data raw data buffer
 * @param frame_data output frame data
 * @return parse state
 */
parse_state_t amqp_parse_frame(enum message_type_t msg_type, struct raw_data_s *raw_data, struct frame_data_s **frame_data);

/**
 * Find frame boundary in raw data
 *
 * @param msg_type message type (request/response)
 * @param raw_data raw data buffer
 * @return frame boundary position
 */
size_t amqp_find_frame_boundary(enum message_type_t msg_type, struct raw_data_s *raw_data);

#endif /* __AMQP_PARSER_H__ */ 