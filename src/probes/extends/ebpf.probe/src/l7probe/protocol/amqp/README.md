# AMQP协议解析器开发文档

## 1. 概述

本文档描述了Gala-Gopher中AMQP协议解析器的实现。AMQP(Advanced Message Queuing Protocol)是一个应用层协议，用于消息中间件。当前实现主要支持AMQP 0-9版本，后续计划支持AMQP 0-10和1.0版本。

## 2. 目录结构

```
amqp/
├── matcher/           # 请求响应匹配器
│   ├── amqp_matcher.h
│   └── amqp_matcher.c
├── model/            # 数据模型定义
│   └── amqp_msg_format.h
├── parser/           # 协议解析器
│   ├── amqp_parser.h
│   └── amqp_parser.c
└── README.md         # 本文档
```

## 3. 功能特性

### 3.1 协议版本支持
- AMQP 0-9 (主要支持)
- AMQP 0-10 (计划支持)
- AMQP 1.0 (计划支持)

### 3.2 帧类型支持
- 方法帧 (Method Frame)
- 内容头帧 (Content Header Frame)
- 内容体帧 (Content Body Frame)
- 心跳帧 (Heartbeat Frame)

### 3.3 类和方法支持
- Connection类方法
  - Connection.Start
  - Connection.Start-Ok
  - Connection.Close
  - Connection.Close-Ok
- Channel类方法
  - Channel.Open
  - Channel.Open-Ok
  - Channel.Close
  - Channel.Close-Ok
- Exchange类方法
  - Exchange.Declare
  - Exchange.Declare-Ok
- Queue类方法
  - Queue.Declare
  - Queue.Declare-Ok
  - Queue.Bind
  - Queue.Bind-Ok
  - Queue.Unbind
  - Queue.Unbind-Ok
  - Queue.Delete
  - Queue.Delete-Ok
- Basic类方法
  - Basic.Qos
  - Basic.Qos-Ok
  - Basic.Consume
  - Basic.Consume-Ok
  - Basic.Cancel
  - Basic.Cancel-Ok
  - Basic.Publish
  - Basic.Return
  - Basic.Deliver
  - Basic.Get
  - Basic.Get-Ok
  - Basic.Get-Empty
  - Basic.Ack
  - Basic.Nack
  - Basic.Recover
  - Basic.Recover-Ok

## 4. 实现细节

### 4.1 数据模型
- 定义了AMQP协议版本、帧类型、类ID和方法ID的枚举
- 实现了消息格式的数据结构
- 支持请求-响应匹配的记录结构

### 4.2 解析器实现
- 实现了帧边界查找
- 支持协议头解析
- 实现了各类帧的解析逻辑
- 支持字符串、数值等基本类型的解析

### 4.3 匹配器实现
- 实现了请求-响应帧的匹配
- 支持状态码的生成
- 实现了内存管理

## 5. 错误处理

- 实现了完整的错误检查机制
- 使用统一的日志格式
- 支持内存分配失败处理
- 实现了协议版本验证

## 6. 内存管理

- 使用calloc分配内存
- 实现了完整的内存释放函数
- 防止内存泄漏
- 支持字符串的动态分配

## 7. 待优化项

1. 支持AMQP 0-10和1.0版本
2. 添加更多测试用例
3. 优化性能
4. 完善错误处理
5. 添加更多文档

## 8. 使用示例

```c
// 解析AMQP帧
parse_state_t state = amqp_parse_frame(msg_type, raw_data, &frame_data);

// 匹配请求响应
amqp_match_frames(req_frame, resp_frame, record_buf);

// 释放资源
free_amqp_record(record);
```

## 9. 贡献指南

1. 遵循现有的代码风格
2. 添加适当的注释
3. 编写单元测试
4. 更新文档

## 10. 许可证

本项目采用Mulan PSL v2许可证。详见LICENSE文件。 