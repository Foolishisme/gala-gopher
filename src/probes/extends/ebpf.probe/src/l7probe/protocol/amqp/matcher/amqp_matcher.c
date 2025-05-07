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
 * Description: AMQP protocol matcher implementation
 ******************************************************************************/

#include <string.h>
#include <stdlib.h>
#include "../../common/protocol_common.h"
#include "amqp_matcher.h"

/* Helper Functions */

static bool is_method_frame(amqp_message_t *msg)
{
    return msg && msg->frame_type == AMQP_0_9_FRAME_TYPE_METHOD;
}

static bool is_content_header_frame(amqp_message_t *msg)
{
    return msg && msg->frame_type == AMQP_0_9_FRAME_TYPE_CONTENT_HEADER;
}

static bool is_content_body_frame(amqp_message_t *msg)
{
    return msg && msg->frame_type == AMQP_0_9_FRAME_TYPE_CONTENT_BODY;
}

static bool is_basic_publish(amqp_message_t *msg)
{
    return is_method_frame(msg) && 
           msg->class_id == AMQP_0_9_CLASS_BASIC &&
           msg->method_id == 40; // Basic.Publish
}

static bool is_basic_deliver(amqp_message_t *msg)
{
    return is_method_frame(msg) && 
           msg->class_id == AMQP_0_9_CLASS_BASIC &&
           msg->method_id == 60; // Basic.Deliver
}

static bool is_basic_ack(amqp_message_t *msg)
{
    return is_method_frame(msg) && 
           msg->class_id == AMQP_0_9_CLASS_BASIC &&
           msg->method_id == 80; // Basic.Ack
}

static bool is_basic_nack(amqp_message_t *msg)
{
    return is_method_frame(msg) && 
           msg->class_id == AMQP_0_9_CLASS_BASIC &&
           msg->method_id == 120; // Basic.Nack
}

static bool is_connection_method(amqp_message_t *msg)
{
    return is_method_frame(msg) && msg->class_id == AMQP_0_9_CLASS_CONNECTION;
}

static bool is_channel_method(amqp_message_t *msg)
{
    return is_method_frame(msg) && msg->class_id == AMQP_0_9_CLASS_CHANNEL;
}

static bool is_exchange_method(amqp_message_t *msg)
{
    return is_method_frame(msg) && msg->class_id == AMQP_0_9_CLASS_EXCHANGE;
}

static bool is_queue_method(amqp_message_t *msg)
{
    return is_method_frame(msg) && msg->class_id == AMQP_0_9_CLASS_QUEUE;
}

static bool is_basic_method(amqp_message_t *msg)
{
    return is_method_frame(msg) && msg->class_id == AMQP_0_9_CLASS_BASIC;
}

static bool is_connection_start(amqp_message_t *msg)
{
    return is_connection_method(msg) && msg->method_id == AMQP_0_9_METHOD_CONNECTION_START;
}

static bool is_connection_start_ok(amqp_message_t *msg)
{
    return is_connection_method(msg) && msg->method_id == AMQP_0_9_METHOD_CONNECTION_START_OK;
}

static bool is_connection_close(amqp_message_t *msg)
{
    return is_connection_method(msg) && msg->method_id == AMQP_0_9_METHOD_CONNECTION_CLOSE;
}

static bool is_connection_close_ok(amqp_message_t *msg)
{
    return is_connection_method(msg) && msg->method_id == AMQP_0_9_METHOD_CONNECTION_CLOSE_OK;
}

static bool is_channel_open(amqp_message_t *msg)
{
    return is_channel_method(msg) && msg->method_id == AMQP_0_9_METHOD_CHANNEL_OPEN;
}

static bool is_channel_open_ok(amqp_message_t *msg)
{
    return is_channel_method(msg) && msg->method_id == AMQP_0_9_METHOD_CHANNEL_OPEN_OK;
}

static bool is_channel_close(amqp_message_t *msg)
{
    return is_channel_method(msg) && msg->method_id == AMQP_0_9_METHOD_CHANNEL_CLOSE;
}

static bool is_channel_close_ok(amqp_message_t *msg)
{
    return is_channel_method(msg) && msg->method_id == AMQP_0_9_METHOD_CHANNEL_CLOSE_OK;
}

static bool is_exchange_declare(amqp_message_t *msg)
{
    return is_exchange_method(msg) && msg->method_id == AMQP_0_9_METHOD_EXCHANGE_DECLARE;
}

static bool is_exchange_declare_ok(amqp_message_t *msg)
{
    return is_exchange_method(msg) && msg->method_id == AMQP_0_9_METHOD_EXCHANGE_DECLARE_OK;
}

static bool is_queue_declare(amqp_message_t *msg)
{
    return is_queue_method(msg) && msg->method_id == AMQP_0_9_METHOD_QUEUE_DECLARE;
}

static bool is_queue_declare_ok(amqp_message_t *msg)
{
    return is_queue_method(msg) && msg->method_id == AMQP_0_9_METHOD_QUEUE_DECLARE_OK;
}

static bool is_queue_bind(amqp_message_t *msg)
{
    return is_queue_method(msg) && msg->method_id == AMQP_0_9_METHOD_QUEUE_BIND;
}

static bool is_queue_bind_ok(amqp_message_t *msg)
{
    return is_queue_method(msg) && msg->method_id == AMQP_0_9_METHOD_QUEUE_BIND_OK;
}

static bool is_queue_unbind(amqp_message_t *msg)
{
    return is_queue_method(msg) && msg->method_id == AMQP_0_9_METHOD_QUEUE_UNBIND;
}

static bool is_queue_unbind_ok(amqp_message_t *msg)
{
    return is_queue_method(msg) && msg->method_id == AMQP_0_9_METHOD_QUEUE_UNBIND_OK;
}

static bool is_queue_delete(amqp_message_t *msg)
{
    return is_queue_method(msg) && msg->method_id == AMQP_0_9_METHOD_QUEUE_DELETE;
}

static bool is_queue_delete_ok(amqp_message_t *msg)
{
    return is_queue_method(msg) && msg->method_id == AMQP_0_9_METHOD_QUEUE_DELETE_OK;
}

static bool is_basic_qos(amqp_message_t *msg)
{
    return is_basic_method(msg) && msg->method_id == AMQP_0_9_METHOD_BASIC_QOS;
}

static bool is_basic_qos_ok(amqp_message_t *msg)
{
    return is_basic_method(msg) && msg->method_id == AMQP_0_9_METHOD_BASIC_QOS_OK;
}

static bool is_basic_consume(amqp_message_t *msg)
{
    return is_basic_method(msg) && msg->method_id == AMQP_0_9_METHOD_BASIC_CONSUME;
}

static bool is_basic_consume_ok(amqp_message_t *msg)
{
    return is_basic_method(msg) && msg->method_id == AMQP_0_9_METHOD_BASIC_CONSUME_OK;
}

static bool is_basic_cancel(amqp_message_t *msg)
{
    return is_basic_method(msg) && msg->method_id == AMQP_0_9_METHOD_BASIC_CANCEL;
}

static bool is_basic_cancel_ok(amqp_message_t *msg)
{
    return is_basic_method(msg) && msg->method_id == AMQP_0_9_METHOD_BASIC_CANCEL_OK;
}

static bool is_basic_get(amqp_message_t *msg)
{
    return is_basic_method(msg) && msg->method_id == AMQP_0_9_METHOD_BASIC_GET;
}

static bool is_basic_get_ok(amqp_message_t *msg)
{
    return is_basic_method(msg) && msg->method_id == AMQP_0_9_METHOD_BASIC_GET_OK;
}

static bool is_basic_get_empty(amqp_message_t *msg)
{
    return is_basic_method(msg) && msg->method_id == AMQP_0_9_METHOD_BASIC_GET_EMPTY;
}

static bool is_basic_recover(amqp_message_t *msg)
{
    return is_basic_method(msg) && msg->method_id == AMQP_0_9_METHOD_BASIC_RECOVER;
}

static bool is_basic_recover_ok(amqp_message_t *msg)
{
    return is_basic_method(msg) && msg->method_id == AMQP_0_9_METHOD_BASIC_RECOVER_OK;
}

/* Public Functions */

void amqp_match_frames(struct frame_buf_s *req_frame, struct frame_buf_s *resp_frame, struct record_buf_s *record_buf)
{
    if (!req_frame || !resp_frame || !record_buf) {
        WARN("[AMQP MATCHER] Invalid parameters.\n");
        return;
    }

    amqp_message_t *req_msg = (amqp_message_t *)req_frame->frame;
    amqp_message_t *resp_msg = (amqp_message_t *)resp_frame->frame;

    if (!req_msg || !resp_msg) {
        WARN("[AMQP MATCHER] Invalid message frames.\n");
        return;
    }

    /* Allocate record */
    amqp_record_t *record = (amqp_record_t *)calloc(1, sizeof(amqp_record_t));
    if (!record) {
        WARN("[AMQP MATCHER] Failed to allocate record.\n");
        return;
    }

    /* Set request message */
    record->req_msg = req_msg;
    record->req_timestamp_ns = req_msg->timestamp_ns;

    /* Set response message */
    record->resp_msg = resp_msg;
    record->resp_timestamp_ns = resp_msg->timestamp_ns;

    /* Set status based on response type */
    if (is_basic_ack(resp_msg) || is_basic_nack(resp_msg)) {
        record->status = is_basic_ack(resp_msg) ? 0 : 1;
    } else if (is_connection_close_ok(resp_msg) || is_channel_close_ok(resp_msg)) {
        record->status = 0;
    } else if (is_connection_close(resp_msg) || is_channel_close(resp_msg)) {
        record->status = 1;
    } else if (is_basic_get_empty(resp_msg)) {
        record->status = 2; // Not found
    } else {
        record->status = -1; // Unknown
        WARN("[AMQP MATCHER] Unknown response type.\n");
    }

    /* Add record to buffer */
    record_buf->record = record;
    record_buf->record_type = PROTO_AMQP;

    DEBUG("[AMQP MATCHER] Successfully matched frames.\n");
}

void free_amqp_record(struct amqp_record_s *record)
{
    if (record == NULL) {
        return;
    }

    /* Free request message */
    if (record->req_msg) {
        free_amqp_msg(record->req_msg);
    }

    /* Free response message */
    if (record->resp_msg) {
        free_amqp_msg(record->resp_msg);
    }

    /* Free record itself */
    free(record);
}

void free_amqp_msg(struct amqp_message_s *msg)
{
    if (msg == NULL) {
        return;
    }

    /* Free message body */
    if (msg->body) {
        free(msg->body);
    }

    /* Free content type */
    if (msg->content_type) {
        free(msg->content_type);
    }

    /* Free content encoding */
    if (msg->content_encoding) {
        free(msg->content_encoding);
    }

    /* Free message itself */
    free(msg);
} 