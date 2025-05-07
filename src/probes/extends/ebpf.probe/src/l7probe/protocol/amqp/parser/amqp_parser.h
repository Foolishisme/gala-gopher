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

#include "../../common/protocol_common.h"
#include "../model/amqp_msg_format.h"

/* AMQP Protocol Constants */
#define AMQP_V0_9 0x09
#define AMQP_V0_10 0x0A
#define AMQP_V1_0 0x10

#define MAX_FRAME_SIZE (128 * 1024 * 1024)  /* 128MB max frame size */
#define MAX_STRING_LENGTH (1024 * 1024)     /* 1MB max string length */

/* AMQP Frame Types */
#define AMQP_0_9_FRAME_TYPE_METHOD 1
#define AMQP_0_9_FRAME_TYPE_CONTENT_HEADER 2
#define AMQP_0_9_FRAME_TYPE_CONTENT_BODY 3
#define AMQP_0_9_FRAME_TYPE_HEARTBEAT 8

/* AMQP Class IDs */
#define AMQP_0_9_CLASS_BASIC 60

/* AMQP Method IDs */
#define AMQP_0_9_METHOD_BASIC_PUBLISH 40
#define AMQP_0_9_METHOD_BASIC_DELIVER 60

/* AMQP Message Structure */
typedef struct {
    uint8_t frame_type;
    uint16_t channel_num;
    uint32_t frame_size;
    uint16_t class_id;
    uint16_t method_id;
    
    char *exchange;
    char *routing_key;
    char *consumer_tag;
    uint64_t delivery_tag;
    bool redelivered;
    
    char *content_type;
    char *content_encoding;
    uint64_t body_size;
    char *body;
    bool body_allocated;
    
    uint64_t timestamp_ns;
} amqp_message_t;

/* Function Declarations */
parse_state_t amqp_parse_frame(enum message_type_t msg_type, struct raw_data_s *raw_data, struct frame_data_s **frame_data);
size_t amqp_find_frame_boundary(enum message_type_t msg_type, struct raw_data_s *raw_data);
void free_amqp_msg(struct amqp_message_s *msg);

#endif /* __AMQP_PARSER_H__ */ 