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

#include "common/protocol_common.h"
#include "../model/amqp_msg_format.h"

/**
 * Parse AMQP frame
 *
 * @param msg_type message type
 * @param raw_data raw data
 * @param frame_data output frame data
 * @return parse state
 */
parse_state_t amqp_parse_frame(enum message_type_t msg_type, struct raw_data_s *raw_data, struct frame_data_s **frame_data);

/**
 * Find frame boundary for AMQP raw data
 *
 * @param msg_type message type
 * @param raw_data raw data
 * @return frame boundary position
 */
size_t amqp_find_frame_boundary(enum message_type_t msg_type, struct raw_data_s *raw_data);

/**
 * Free AMQP message data
 *
 * @param msg AMQP message to be freed
 */
void free_amqp_msg(struct amqp_message_s *msg);

#endif /* __AMQP_PARSER_H__ */ 