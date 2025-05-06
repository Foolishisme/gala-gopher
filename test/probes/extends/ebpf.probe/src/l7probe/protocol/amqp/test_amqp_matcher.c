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
 * Description: AMQP protocol matcher test cases
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "common/protocol_common.h"
#include "amqp_matcher.h"

/* Test Data */
static const char AMQP_PUBLISH[] = "\x01\x00\x01\x00\x00\x00\x0C\x00\x3C\x00\x28\x00\x00\x00\x00\x00\x00\xCE";
static const char AMQP_ACK[] = "\x01\x00\x01\x00\x00\x00\x0C\x00\x3C\x00\x50\x00\x00\x00\x00\x00\x00\xCE";
static const char AMQP_NACK[] = "\x01\x00\x01\x00\x00\x00\x0C\x00\x3C\x00\x78\x00\x00\x00\x00\x00\x00\xCE";
static const char AMQP_GET[] = "\x01\x00\x01\x00\x00\x00\x0C\x00\x3C\x00\x46\x00\x00\x00\x00\x00\x00\xCE";
static const char AMQP_GET_EMPTY[] = "\x01\x00\x01\x00\x00\x00\x0C\x00\x3C\x00\x48\x00\x00\x00\x00\x00\x00\xCE";

/* Test Functions */

static void test_publish_ack(void)
{
    struct frame_buf_s req_frame = {0};
    struct frame_buf_s resp_frame = {0};
    struct record_buf_s record_buf = {0};
    
    /* Setup request frame */
    req_frame.frame = (void *)AMQP_PUBLISH;
    req_frame.msg_type = MSG_TYPE_REQUEST;
    req_frame.timestamp_ns = 1000;
    
    /* Setup response frame */
    resp_frame.frame = (void *)AMQP_ACK;
    resp_frame.msg_type = MSG_TYPE_RESPONSE;
    resp_frame.timestamp_ns = 2000;
    
    /* Test matching */
    amqp_match_frames(&req_frame, &resp_frame, &record_buf);
    
    /* Verify result */
    assert(record_buf.record != NULL);
    assert(record_buf.record_type == PROTO_AMQP);
    
    amqp_record_t *record = (amqp_record_t *)record_buf.record;
    assert(record->req_timestamp_ns == 1000);
    assert(record->resp_timestamp_ns == 2000);
    assert(record->status == 0); // Success
    
    /* Cleanup */
    if (record_buf.record) {
        free(record_buf.record);
    }
}

static void test_publish_nack(void)
{
    struct frame_buf_s req_frame = {0};
    struct frame_buf_s resp_frame = {0};
    struct record_buf_s record_buf = {0};
    
    /* Setup request frame */
    req_frame.frame = (void *)AMQP_PUBLISH;
    req_frame.msg_type = MSG_TYPE_REQUEST;
    req_frame.timestamp_ns = 1000;
    
    /* Setup response frame */
    resp_frame.frame = (void *)AMQP_NACK;
    resp_frame.msg_type = MSG_TYPE_RESPONSE;
    resp_frame.timestamp_ns = 2000;
    
    /* Test matching */
    amqp_match_frames(&req_frame, &resp_frame, &record_buf);
    
    /* Verify result */
    assert(record_buf.record != NULL);
    assert(record_buf.record_type == PROTO_AMQP);
    
    amqp_record_t *record = (amqp_record_t *)record_buf.record;
    assert(record->req_timestamp_ns == 1000);
    assert(record->resp_timestamp_ns == 2000);
    assert(record->status == 1); // Error
    
    /* Cleanup */
    if (record_buf.record) {
        free(record_buf.record);
    }
}

static void test_get_empty(void)
{
    struct frame_buf_s req_frame = {0};
    struct frame_buf_s resp_frame = {0};
    struct record_buf_s record_buf = {0};
    
    /* Setup request frame */
    req_frame.frame = (void *)AMQP_GET;
    req_frame.msg_type = MSG_TYPE_REQUEST;
    req_frame.timestamp_ns = 1000;
    
    /* Setup response frame */
    resp_frame.frame = (void *)AMQP_GET_EMPTY;
    resp_frame.msg_type = MSG_TYPE_RESPONSE;
    resp_frame.timestamp_ns = 2000;
    
    /* Test matching */
    amqp_match_frames(&req_frame, &resp_frame, &record_buf);
    
    /* Verify result */
    assert(record_buf.record != NULL);
    assert(record_buf.record_type == PROTO_AMQP);
    
    amqp_record_t *record = (amqp_record_t *)record_buf.record;
    assert(record->req_timestamp_ns == 1000);
    assert(record->resp_timestamp_ns == 2000);
    assert(record->status == 2); // Not found
    
    /* Cleanup */
    if (record_buf.record) {
        free(record_buf.record);
    }
}

static void test_null_frames(void)
{
    struct record_buf_s record_buf = {0};
    
    /* Test with NULL request frame */
    amqp_match_frames(NULL, NULL, &record_buf);
    assert(record_buf.record == NULL);
    
    /* Test with NULL response frame */
    struct frame_buf_s req_frame = {0};
    amqp_match_frames(&req_frame, NULL, &record_buf);
    assert(record_buf.record == NULL);
    
    /* Test with NULL record buffer */
    struct frame_buf_s resp_frame = {0};
    amqp_match_frames(&req_frame, &resp_frame, NULL);
    assert(record_buf.record == NULL);
}

/* Main Test Function */

int main(void)
{
    printf("Running AMQP matcher tests...\n");
    
    test_publish_ack();
    test_publish_nack();
    test_get_empty();
    test_null_frames();
    
    printf("All tests passed!\n");
    return 0;
} 