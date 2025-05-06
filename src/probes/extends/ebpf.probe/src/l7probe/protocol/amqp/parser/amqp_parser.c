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
 * Description: AMQP protocol parser implementation
 ******************************************************************************/

#include <string.h>
#include <stdlib.h>
#include "common/protocol_common.h"
#include "utils/binary_decoder.h"
#include "amqp_parser.h"

/* AMQP Protocol Constants */
#define AMQP_FRAME_HEADER_SIZE 8
#define AMQP_FRAME_END 0xCE
#define AMQP_PROTOCOL_HEADER "AMQP"

/* Helper Functions */

static bool is_protocol_header(const char *data, size_t len)
{
    return (len >= 4 && memcmp(data, AMQP_PROTOCOL_HEADER, 4) == 0);
}

static uint8_t detect_amqp_version(const char *data, size_t len)
{
    if (len < 8) {
        return 0;
    }

    if (is_protocol_header(data, len)) {
        uint8_t major = data[6];
        uint8_t minor = data[7];
        
        if (major == 1 && minor == 0) {
            return AMQP_V1_0;
        } else if (minor == 9) {
            return AMQP_V0_9;
        } else if (minor == 10) {
            return AMQP_V0_10;
        }
    }
    
    return 0;
}

/* Helper Functions for AMQP 0-9 */

static void parse_short_string(const char *data, size_t *pos, char **str)
{
    uint8_t len = data[*pos];
    (*pos)++;
    if (len > 0) {
        *str = (char *)malloc(len + 1);
        if (*str) {
            memcpy(*str, data + *pos, len);
            (*str)[len] = '\0';
        }
        *pos += len;
    }
}

static void parse_long_string(const char *data, size_t *pos, char **str)
{
    uint32_t len = (data[*pos] << 24) | (data[*pos + 1] << 16) |
                   (data[*pos + 2] << 8) | data[*pos + 3];
    *pos += 4;
    if (len > 0) {
        *str = (char *)malloc(len + 1);
        if (*str) {
            memcpy(*str, data + *pos, len);
            (*str)[len] = '\0';
        }
        *pos += len;
    }
}

static parse_state_t parse_basic_publish(const char *data, size_t *pos, amqp_message_t *msg)
{
    /* Skip reserved field */
    *pos += 2;
    
    /* Parse exchange name */
    parse_short_string(data, pos, &msg->exchange);
    
    /* Parse routing key */
    parse_short_string(data, pos, &msg->routing_key);
    
    /* Skip mandatory and immediate flags */
    *pos += 2;
    
    return STATE_SUCCESS;
}

static parse_state_t parse_basic_deliver(const char *data, size_t *pos, amqp_message_t *msg)
{
    /* Parse consumer tag */
    parse_short_string(data, pos, &msg->consumer_tag);
    
    /* Parse delivery tag */
    msg->delivery_tag = (data[*pos] << 56) | (data[*pos + 1] << 48) |
                       (data[*pos + 2] << 40) | (data[*pos + 3] << 32) |
                       (data[*pos + 4] << 24) | (data[*pos + 5] << 16) |
                       (data[*pos + 6] << 8) | data[*pos + 7];
    *pos += 8;
    
    /* Parse redelivered flag */
    msg->redelivered = data[*pos] != 0;
    *pos += 1;
    
    /* Parse exchange name */
    parse_short_string(data, pos, &msg->exchange);
    
    /* Parse routing key */
    parse_short_string(data, pos, &msg->routing_key);
    
    return STATE_SUCCESS;
}

static parse_state_t parse_content_header(const char *data, size_t *pos, amqp_message_t *msg)
{
    /* Skip class ID (already known) */
    *pos += 2;
    
    /* Skip weight */
    *pos += 2;
    
    /* Parse body size */
    msg->body_size = (data[*pos] << 56) | (data[*pos + 1] << 48) |
                    (data[*pos + 2] << 40) | (data[*pos + 3] << 32) |
                    (data[*pos + 4] << 24) | (data[*pos + 5] << 16) |
                    (data[*pos + 6] << 8) | data[*pos + 7];
    *pos += 8;
    
    /* Parse property flags */
    uint16_t flags = (data[*pos] << 8) | data[*pos + 1];
    *pos += 2;
    
    /* Parse properties based on flags */
    if (flags & 0x8000) { /* content-type */
        parse_short_string(data, pos, &msg->content_type);
    }
    if (flags & 0x4000) { /* content-encoding */
        parse_short_string(data, pos, &msg->content_encoding);
    }
    if (flags & 0x2000) { /* headers */
        /* Skip headers for now */
        *pos += 4;
    }
    if (flags & 0x1000) { /* delivery-mode */
        *pos += 1;
    }
    if (flags & 0x0800) { /* priority */
        *pos += 1;
    }
    if (flags & 0x0400) { /* correlation-id */
        parse_short_string(data, pos, NULL);
    }
    if (flags & 0x0200) { /* reply-to */
        parse_short_string(data, pos, NULL);
    }
    if (flags & 0x0100) { /* expiration */
        parse_short_string(data, pos, NULL);
    }
    if (flags & 0x0080) { /* message-id */
        parse_short_string(data, pos, NULL);
    }
    if (flags & 0x0040) { /* timestamp */
        *pos += 8;
    }
    if (flags & 0x0020) { /* type */
        parse_short_string(data, pos, NULL);
    }
    if (flags & 0x0010) { /* user-id */
        parse_short_string(data, pos, NULL);
    }
    if (flags & 0x0008) { /* app-id */
        parse_short_string(data, pos, NULL);
    }
    if (flags & 0x0004) { /* cluster-id */
        parse_short_string(data, pos, NULL);
    }
    
    return STATE_SUCCESS;
}

static parse_state_t parse_amqp_0_9_frame(struct raw_data_s *raw_data, amqp_message_t *msg)
{
    if (raw_data->data_len < AMQP_FRAME_HEADER_SIZE) {
        return STATE_NEEDS_MORE_DATA;
    }

    /* Parse frame header */
    msg->frame_type = raw_data->data[0];
    msg->channel_num = (raw_data->data[1] << 8) | raw_data->data[2];
    msg->frame_size = (raw_data->data[3] << 24) | (raw_data->data[4] << 16) |
                     (raw_data->data[5] << 8) | raw_data->data[6];

    /* Check if we have enough data for the complete frame */
    if (raw_data->data_len < msg->frame_size + AMQP_FRAME_HEADER_SIZE) {
        return STATE_NEEDS_MORE_DATA;
    }

    /* Check frame end marker */
    if (raw_data->data[msg->frame_size + AMQP_FRAME_HEADER_SIZE - 1] != AMQP_FRAME_END) {
        return STATE_INVALID;
    }

    size_t pos = AMQP_FRAME_HEADER_SIZE;

    /* Parse frame body based on frame type */
    switch (msg->frame_type) {
        case AMQP_0_9_FRAME_TYPE_METHOD:
            if (msg->frame_size < 4) {
                return STATE_INVALID;
            }
            msg->class_id = raw_data->data[pos++];
            msg->method_id = (raw_data->data[pos] << 8) | raw_data->data[pos + 1];
            pos += 2;

            /* Parse method arguments based on class and method */
            if (msg->class_id == AMQP_0_9_CLASS_BASIC) {
                if (msg->method_id == AMQP_0_9_METHOD_BASIC_PUBLISH) {
                    parse_basic_publish(raw_data->data, &pos, msg);
                } else if (msg->method_id == AMQP_0_9_METHOD_BASIC_DELIVER) {
                    parse_basic_deliver(raw_data->data, &pos, msg);
                }
            }
            break;

        case AMQP_0_9_FRAME_TYPE_CONTENT_HEADER:
            parse_content_header(raw_data->data, &pos, msg);
            break;

        case AMQP_0_9_FRAME_TYPE_CONTENT_BODY:
            msg->body = (char *)raw_data->data + pos;
            msg->body_size = msg->frame_size;
            break;

        case AMQP_0_9_FRAME_TYPE_HEARTBEAT:
            /* Heartbeat frames have no body */
            break;

        default:
            return STATE_INVALID;
    }

    /* Update current position */
    raw_data->current_pos += msg->frame_size + AMQP_FRAME_HEADER_SIZE;

    return STATE_SUCCESS;
}

/* Public Functions */

parse_state_t amqp_parse_frame(enum message_type_t msg_type, struct raw_data_s *raw_data, struct frame_data_s **frame_data)
{
    if (raw_data == NULL || frame_data == NULL) {
        WARN("[AMQP PARSER] Invalid parameters.\n");
        return STATE_INVALID;
    }

    /* Check for protocol header */
    if (is_protocol_header(raw_data->data, raw_data->data_len)) {
        uint8_t version = detect_amqp_version(raw_data->data, raw_data->data_len);
        if (version == 0) {
            WARN("[AMQP PARSER] Invalid protocol version.\n");
            return STATE_INVALID;
        }
        raw_data->current_pos += 8; // Skip protocol header
        return STATE_SUCCESS;
    }

    /* Allocate frame data structure */
    *frame_data = (struct frame_data_s *)calloc(1, sizeof(struct frame_data_s));
    if (*frame_data == NULL) {
        WARN("[AMQP PARSER] Failed to allocate frame data.\n");
        return STATE_INVALID;
    }

    /* Allocate AMQP message structure */
    amqp_message_t *msg = (amqp_message_t *)calloc(1, sizeof(amqp_message_t));
    if (msg == NULL) {
        WARN("[AMQP PARSER] Failed to allocate message.\n");
        free(*frame_data);
        return STATE_INVALID;
    }

    /* Set message type and timestamp */
    msg->timestamp_ns = raw_data->timestamp_ns;

    /* Parse frame based on version */
    parse_state_t state = parse_amqp_0_9_frame(raw_data, msg);
    if (state != STATE_SUCCESS) {
        WARN("[AMQP PARSER] Failed to parse frame.\n");
        free(msg);
        free(*frame_data);
        return state;
    }

    /* Set frame data */
    (*frame_data)->frame = msg;
    (*frame_data)->msg_type = msg_type;
    (*frame_data)->timestamp_ns = msg->timestamp_ns;

    DEBUG("[AMQP PARSER] Successfully parsed frame.\n");
    return STATE_SUCCESS;
}

size_t amqp_find_frame_boundary(enum message_type_t msg_type, struct raw_data_s *raw_data)
{
    if (raw_data == NULL || raw_data->data == NULL || raw_data->data_len == 0) {
        return 0;
    }

    /* Check for protocol header */
    if (is_protocol_header(raw_data->data, raw_data->data_len)) {
        return 8; // Protocol header size
    }

    /* Check for frame header */
    if (raw_data->data_len < AMQP_FRAME_HEADER_SIZE) {
        return 0;
    }

    /* Get frame size from header */
    uint32_t frame_size = (raw_data->data[3] << 24) | (raw_data->data[4] << 16) |
                         (raw_data->data[5] << 8) | raw_data->data[6];

    /* Check if we have enough data for the complete frame */
    if (raw_data->data_len < frame_size + AMQP_FRAME_HEADER_SIZE) {
        return 0;
    }

    /* Check frame end marker */
    if (raw_data->data[frame_size + AMQP_FRAME_HEADER_SIZE - 1] != AMQP_FRAME_END) {
        return 0;
    }

    return frame_size + AMQP_FRAME_HEADER_SIZE;
} 