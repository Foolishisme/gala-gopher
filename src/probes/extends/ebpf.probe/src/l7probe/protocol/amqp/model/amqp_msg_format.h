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
 * Description: AMQP protocol message format definitions
 ******************************************************************************/
#ifndef __AMQP_MSG_FORMAT_H__
#define __AMQP_MSG_FORMAT_H__

#include <stdint.h>
#include <stdbool.h>

/* AMQP Protocol Versions */
#define AMQP_V0_9           2
#define AMQP_V0_10          4
#define AMQP_V1_0           5

/* AMQP 0-9 Frame Types */
#define AMQP_0_9_FRAME_TYPE_METHOD             1
#define AMQP_0_9_FRAME_TYPE_CONTENT_HEADER     2
#define AMQP_0_9_FRAME_TYPE_CONTENT_BODY       3
#define AMQP_0_9_FRAME_TYPE_HEARTBEAT          8

/* AMQP 0-9 Class IDs */
#define AMQP_0_9_CLASS_CONNECTION    10
#define AMQP_0_9_CLASS_CHANNEL       20
#define AMQP_0_9_CLASS_EXCHANGE      40
#define AMQP_0_9_CLASS_QUEUE         50
#define AMQP_0_9_CLASS_BASIC         60
#define AMQP_0_9_CLASS_FILE          70
#define AMQP_0_9_CLASS_STREAM        80
#define AMQP_0_9_CLASS_TUNNEL        90

/* AMQP 0-9 Method IDs */
/* Connection Methods */
#define AMQP_0_9_METHOD_CONNECTION_START       10
#define AMQP_0_9_METHOD_CONNECTION_START_OK    11
#define AMQP_0_9_METHOD_CONNECTION_SECURE      20
#define AMQP_0_9_METHOD_CONNECTION_SECURE_OK   21
#define AMQP_0_9_METHOD_CONNECTION_TUNE        30
#define AMQP_0_9_METHOD_CONNECTION_TUNE_OK     31
#define AMQP_0_9_METHOD_CONNECTION_OPEN        40
#define AMQP_0_9_METHOD_CONNECTION_OPEN_OK     41
#define AMQP_0_9_METHOD_CONNECTION_CLOSE       50
#define AMQP_0_9_METHOD_CONNECTION_CLOSE_OK    51

/* Channel Methods */
#define AMQP_0_9_METHOD_CHANNEL_OPEN           10
#define AMQP_0_9_METHOD_CHANNEL_OPEN_OK        11
#define AMQP_0_9_METHOD_CHANNEL_FLOW           20
#define AMQP_0_9_METHOD_CHANNEL_FLOW_OK        21
#define AMQP_0_9_METHOD_CHANNEL_CLOSE          40
#define AMQP_0_9_METHOD_CHANNEL_CLOSE_OK       41

/* Exchange Methods */
#define AMQP_0_9_METHOD_EXCHANGE_DECLARE       10
#define AMQP_0_9_METHOD_EXCHANGE_DECLARE_OK    11
#define AMQP_0_9_METHOD_EXCHANGE_DELETE        20
#define AMQP_0_9_METHOD_EXCHANGE_DELETE_OK     21

/* Queue Methods */
#define AMQP_0_9_METHOD_QUEUE_DECLARE          10
#define AMQP_0_9_METHOD_QUEUE_DECLARE_OK       11
#define AMQP_0_9_METHOD_QUEUE_BIND             20
#define AMQP_0_9_METHOD_QUEUE_BIND_OK          21
#define AMQP_0_9_METHOD_QUEUE_PURGE            30
#define AMQP_0_9_METHOD_QUEUE_PURGE_OK         31
#define AMQP_0_9_METHOD_QUEUE_DELETE           40
#define AMQP_0_9_METHOD_QUEUE_DELETE_OK        41
#define AMQP_0_9_METHOD_QUEUE_UNBIND           50
#define AMQP_0_9_METHOD_QUEUE_UNBIND_OK        51

/* Basic Methods */
#define AMQP_0_9_METHOD_BASIC_QOS              10
#define AMQP_0_9_METHOD_BASIC_QOS_OK           11
#define AMQP_0_9_METHOD_BASIC_CONSUME          20
#define AMQP_0_9_METHOD_BASIC_CONSUME_OK       21
#define AMQP_0_9_METHOD_BASIC_CANCEL           30
#define AMQP_0_9_METHOD_BASIC_CANCEL_OK        31
#define AMQP_0_9_METHOD_BASIC_PUBLISH          40
#define AMQP_0_9_METHOD_BASIC_RETURN           50
#define AMQP_0_9_METHOD_BASIC_DELIVER          60
#define AMQP_0_9_METHOD_BASIC_GET              70
#define AMQP_0_9_METHOD_BASIC_GET_OK           71
#define AMQP_0_9_METHOD_BASIC_GET_EMPTY        72
#define AMQP_0_9_METHOD_BASIC_ACK              80
#define AMQP_0_9_METHOD_BASIC_REJECT           90
#define AMQP_0_9_METHOD_BASIC_RECOVER          100
#define AMQP_0_9_METHOD_BASIC_RECOVER_OK       101
#define AMQP_0_9_METHOD_BASIC_NACK             120

/* AMQP Message Structure */
typedef struct {
    uint8_t version;           /* AMQP protocol version */
    uint8_t frame_type;        /* Frame type */
    uint16_t channel_num;      /* Channel number */
    uint32_t frame_size;       /* Frame size */
    uint8_t class_id;          /* Class ID */
    uint16_t method_id;        /* Method ID */
    char *content_type;        /* Content type */
    char *content_encoding;    /* Content encoding */
    uint64_t delivery_tag;     /* Delivery tag */
    bool redelivered;          /* Redelivered flag */
    char *exchange;            /* Exchange name */
    char *routing_key;         /* Routing key */
    char *queue;               /* Queue name */
    char *consumer_tag;        /* Consumer tag */
    uint64_t timestamp_ns;     /* Timestamp in nanoseconds */
    char *body;                /* Message body */
    size_t body_size;          /* Message body size */
} amqp_message_t;

/* AMQP Record Structure */
typedef struct {
    amqp_message_t *req_msg;   /* Request message */
    amqp_message_t *resp_msg;  /* Response message */
    uint64_t req_timestamp_ns; /* Request timestamp */
    uint64_t resp_timestamp_ns;/* Response timestamp */
    int32_t status;            /* Response status */
} amqp_record_t;

#endif /* __AMQP_MSG_FORMAT_H__ */ 