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
 * Description: AMQP protocol parser test cases
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "common/protocol_common.h"
#include "amqp_parser.h"

/* Test Data */
static const char AMQP_PROTOCOL_HEADER[] = "AMQP\x00\x00\x09\x01";
static const char AMQP_METHOD_FRAME[] = "\x01\x00\x01\x00\x00\x00\x0C\x00\x3C\x00\x0A\x00\x00\x00\x00\x00\x00\xCE";
static const char AMQP_CONTENT_HEADER[] = "\x02\x00\x01\x00\x00\x00\x0C\x00\x3C\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xCE";
static const char AMQP_CONTENT_BODY[] = "\x03\x00\x01\x00\x00\x00\x05\x48\x65\x6C\x6C\x6F\xCE";
static const char AMQP_HEARTBEAT[] = "\x08\x00\x00\x00\x00\x00\x00\x00\xCE";

/* Test Functions */

static void test_protocol_header(void)
{
    struct raw_data_s raw_data;
    struct frame_data_s *frame_data = NULL;
    
    /* Setup test data */
    raw_data.data = (char *)AMQP_PROTOCOL_HEADER;
    raw_data.data_len = sizeof(AMQP_PROTOCOL_HEADER) - 1;
    raw_data.current_pos = 0;
    raw_data.timestamp_ns = 0;
    
    /* Test protocol header parsing */
    parse_state_t state = amqp_parse_frame(MSG_TYPE_REQUEST, &raw_data, &frame_data);
    assert(state == STATE_SUCCESS);
    assert(raw_data.current_pos == 8);
    
    /* Cleanup */
    if (frame_data) {
        free(frame_data);
    }
}

static void test_method_frame(void)
{
    struct raw_data_s raw_data;
    struct frame_data_s *frame_data = NULL;
    
    /* Setup test data */
    raw_data.data = (char *)AMQP_METHOD_FRAME;
    raw_data.data_len = sizeof(AMQP_METHOD_FRAME) - 1;
    raw_data.current_pos = 0;
    raw_data.timestamp_ns = 0;
    
    /* Test method frame parsing */
    parse_state_t state = amqp_parse_frame(MSG_TYPE_REQUEST, &raw_data, &frame_data);
    assert(state == STATE_SUCCESS);
    assert(frame_data != NULL);
    
    amqp_message_t *msg = (amqp_message_t *)frame_data->frame;
    assert(msg != NULL);
    assert(msg->frame_type == AMQP_0_9_FRAME_TYPE_METHOD);
    assert(msg->channel_num == 1);
    assert(msg->class_id == AMQP_0_9_CLASS_CONNECTION);
    assert(msg->method_id == AMQP_0_9_METHOD_CONNECTION_START);
    
    /* Cleanup */
    if (frame_data) {
        free(frame_data);
    }
}

static void test_content_header(void)
{
    struct raw_data_s raw_data;
    struct frame_data_s *frame_data = NULL;
    
    /* Setup test data */
    raw_data.data = (char *)AMQP_CONTENT_HEADER;
    raw_data.data_len = sizeof(AMQP_CONTENT_HEADER) - 1;
    raw_data.current_pos = 0;
    raw_data.timestamp_ns = 0;
    
    /* Test content header parsing */
    parse_state_t state = amqp_parse_frame(MSG_TYPE_REQUEST, &raw_data, &frame_data);
    assert(state == STATE_SUCCESS);
    assert(frame_data != NULL);
    
    amqp_message_t *msg = (amqp_message_t *)frame_data->frame;
    assert(msg != NULL);
    assert(msg->frame_type == AMQP_0_9_FRAME_TYPE_CONTENT_HEADER);
    assert(msg->channel_num == 1);
    assert(msg->body_size == 0);
    
    /* Cleanup */
    if (frame_data) {
        free(frame_data);
    }
}

static void test_content_body(void)
{
    struct raw_data_s raw_data;
    struct frame_data_s *frame_data = NULL;
    
    /* Setup test data */
    raw_data.data = (char *)AMQP_CONTENT_BODY;
    raw_data.data_len = sizeof(AMQP_CONTENT_BODY) - 1;
    raw_data.current_pos = 0;
    raw_data.timestamp_ns = 0;
    
    /* Test content body parsing */
    parse_state_t state = amqp_parse_frame(MSG_TYPE_REQUEST, &raw_data, &frame_data);
    assert(state == STATE_SUCCESS);
    assert(frame_data != NULL);
    
    amqp_message_t *msg = (amqp_message_t *)frame_data->frame;
    assert(msg != NULL);
    assert(msg->frame_type == AMQP_0_9_FRAME_TYPE_CONTENT_BODY);
    assert(msg->channel_num == 1);
    assert(msg->body_size == 5);
    assert(memcmp(msg->body, "Hello", 5) == 0);
    
    /* Cleanup */
    if (frame_data) {
        free(frame_data);
    }
}

static void test_heartbeat(void)
{
    struct raw_data_s raw_data;
    struct frame_data_s *frame_data = NULL;
    
    /* Setup test data */
    raw_data.data = (char *)AMQP_HEARTBEAT;
    raw_data.data_len = sizeof(AMQP_HEARTBEAT) - 1;
    raw_data.current_pos = 0;
    raw_data.timestamp_ns = 0;
    
    /* Test heartbeat parsing */
    parse_state_t state = amqp_parse_frame(MSG_TYPE_REQUEST, &raw_data, &frame_data);
    assert(state == STATE_SUCCESS);
    assert(frame_data != NULL);
    
    amqp_message_t *msg = (amqp_message_t *)frame_data->frame;
    assert(msg != NULL);
    assert(msg->frame_type == AMQP_0_9_FRAME_TYPE_HEARTBEAT);
    assert(msg->channel_num == 0);
    
    /* Cleanup */
    if (frame_data) {
        free(frame_data);
    }
}

static void test_invalid_frame(void)
{
    struct raw_data_s raw_data;
    struct frame_data_s *frame_data = NULL;
    
    /* Setup test data with invalid frame type */
    char invalid_frame[] = "\x09\x00\x01\x00\x00\x00\x00\xCE";
    raw_data.data = invalid_frame;
    raw_data.data_len = sizeof(invalid_frame) - 1;
    raw_data.current_pos = 0;
    raw_data.timestamp_ns = 0;
    
    /* Test invalid frame parsing */
    parse_state_t state = amqp_parse_frame(MSG_TYPE_REQUEST, &raw_data, &frame_data);
    assert(state == STATE_INVALID);
    assert(frame_data == NULL);
}

static void test_incomplete_frame(void)
{
    struct raw_data_s raw_data;
    struct frame_data_s *frame_data = NULL;
    
    /* Setup test data with incomplete frame */
    char incomplete_frame[] = "\x01\x00\x01\x00\x00\x00\x0C";
    raw_data.data = incomplete_frame;
    raw_data.data_len = sizeof(incomplete_frame) - 1;
    raw_data.current_pos = 0;
    raw_data.timestamp_ns = 0;
    
    /* Test incomplete frame parsing */
    parse_state_t state = amqp_parse_frame(MSG_TYPE_REQUEST, &raw_data, &frame_data);
    assert(state == STATE_NEEDS_MORE_DATA);
    assert(frame_data == NULL);
}

/* Main Test Function */

int main(void)
{
    printf("Running AMQP parser tests...\n");
    
    test_protocol_header();
    test_method_frame();
    test_content_header();
    test_content_body();
    test_heartbeat();
    test_invalid_frame();
    test_incomplete_frame();
    
    printf("All tests passed!\n");
    return 0;
} 