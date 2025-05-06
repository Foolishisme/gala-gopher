/*******************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * gala-gopher licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Author: zhaoguolin
 * Create: 2023-04-15
 * Description:
 ******************************************************************************/

#ifndef __PROTOCOL_COMMON_H__
#define __PROTOCOL_COMMON_H__

#pragma once

#include "data_stream.h"
#include <stdint.h>
#include <stdbool.h>

/* Protocol Types */
enum proto_type_t {
    PROTO_UNKNOWN = 0,
    PROTO_HTTP = 1,
    PROTO_HTTP2 = 2,
    PROTO_REDIS = 3,
    PROTO_KAFKA = 4,
    PROTO_PGSQL = 5,
    PROTO_MYSQL = 6,
    PROTO_MONGO = 7,
    PROTO_DNS = 8,
    PROTO_NATS = 9,
    PROTO_CQL = 10,
    PROTO_CRPC = 11,
    PROTO_AMQP = 12,
    PROTO_MAX
};

/* Message Types */
enum message_type_t {
    MESSAGE_UNKNOWN = 0,
    MESSAGE_REQUEST = 1,
    MESSAGE_RESPONSE = 2,
    MESSAGE_MAX
};

/* Parse States */
enum parse_state_t {
    STATE_UNKNOWN = 0,
    STATE_SUCCESS = 1,
    STATE_NEEDS_MORE_DATA = 2,
    STATE_INVALID = 3,
    STATE_MAX
};

/* Raw Data Structure */
struct raw_data_s {
    char *data;
    size_t data_len;
    size_t current_pos;
    uint64_t timestamp_ns;
};

/* Frame Data Structure */
struct frame_data_s {
    void *frame;
    enum message_type_t msg_type;
    uint64_t timestamp_ns;
};

/* Frame Buffer Structure */
struct frame_buf_s {
    void *frame;
    size_t frame_buf_size;
    enum message_type_t msg_type;
    uint64_t timestamp_ns;
};

/* Record Buffer Structure */
struct record_buf_s {
    void *record;
    enum proto_type_t record_type;
    uint64_t req_timestamp_ns;
    uint64_t resp_timestamp_ns;
    uint32_t err_count;
};

/**
 * 拷贝raw_data_s。
 *
 * @param raw_data 字符串缓存
 * @return raw_data_s *
 */
struct raw_data_s *parser_copy_raw_data(struct raw_data_s *raw_data);

/**
 * 根据字符串初始化raw_data_s
 *
 * @param str 字符串首地址
 * @param str_len 字符串长度
 * @return struct raw_data_s *
 */
struct raw_data_s *init_raw_data_with_str(char *str, size_t str_len);

/**
 * 偏移字符串缓存raw_data当前首地址
 *
 * @param raw_data 字符串缓存
 * @param offset 偏移量
 */
void parser_raw_data_offset(struct raw_data_s *raw_data, size_t offset);

#endif
