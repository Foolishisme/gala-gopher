# Gala-Gopher协议解析开发指南

## 1. 概述

本文档描述了在Gala-Gopher中添加新协议解析器的开发流程和规范。每个协议解析器都需要遵循统一的接口和实现规范，以确保与现有系统的兼容性。

## 2. 目录结构

新协议解析器应遵循以下目录结构：

```
protocol/
├── new_protocol/           # 协议名称目录
│   ├── matcher/           # 请求响应匹配器
│   │   ├── new_protocol_matcher.h
│   │   └── new_protocol_matcher.c
│   ├── model/            # 数据模型定义
│   │   └── new_protocol_msg_format.h
│   ├── parser/           # 协议解析器
│   │   ├── new_protocol_parser.h
│   │   └── new_protocol_parser.c
│   └── README.md         # 协议文档
```

## 3. 开发步骤

### 3.1 注册协议类型

在`protocol_common.h`中添加新协议类型：

```c
enum proto_type_t {
    // ... 现有协议类型 ...
    PROTO_NEW_PROTOCOL = N,  // N为下一个可用序号
    PROTO_MAX
};
```

### 3.2 定义数据模型

在`model/new_protocol_msg_format.h`中定义：

1. 协议版本枚举
2. 消息类型枚举
3. 帧类型枚举
4. 消息结构体
5. 记录结构体

### 3.3 实现解析器

在`parser/new_protocol_parser.h`中定义接口：

```c
parse_state_t new_protocol_parse_frame(enum message_type_t msg_type, 
                                     struct raw_data_s *raw_data,
                                     struct frame_data_s **frame_data);

size_t new_protocol_find_frame_boundary(enum message_type_t msg_type,
                                      struct raw_data_s *raw_data);

void free_new_protocol_msg(struct new_protocol_message_s *msg);
```

在`parser/new_protocol_parser.c`中实现：

1. 帧边界查找
2. 协议头解析
3. 消息解析
4. 内存管理

### 3.4 实现匹配器

在`matcher/new_protocol_matcher.h`中定义接口：

```c
void new_protocol_match_frames(struct frame_buf_s *req_frame,
                             struct frame_buf_s *resp_frame,
                             struct record_buf_s *record_buf);

void free_new_protocol_record(struct new_protocol_record_s *record);
```

在`matcher/new_protocol_matcher.c`中实现：

1. 请求响应匹配逻辑
2. 状态码生成
3. 内存管理

### 3.5 集成到协议解析器

在`protocol_parser.c`中添加：

1. 头文件包含
2. 内存释放函数
3. 帧解析函数
4. 帧匹配函数

## 4. 实现规范

### 4.1 错误处理

- 使用统一的错误状态码
- 实现完整的参数检查
- 使用统一的日志格式
- 处理内存分配失败

### 4.2 内存管理

- 使用`calloc`分配内存
- 实现完整的内存释放函数
- 防止内存泄漏
- 检查空指针

### 4.3 日志记录

- 使用统一的日志级别
- 统一的日志前缀格式
- 记录关键操作状态
- 记录错误信息

### 4.4 代码风格

- 遵循现有的代码风格
- 添加适当的注释
- 使用统一的命名规范
- 保持代码简洁清晰

## 5. 测试要求

1. 单元测试
   - 基本功能测试
   - 边界条件测试
   - 错误处理测试
   - 内存管理测试

2. 集成测试
   - 与其他协议交互测试
   - 性能测试
   - 稳定性测试

## 6. 文档要求

1. 协议文档
   - 协议概述
   - 协议版本支持
   - 帧格式说明
   - 消息类型说明

2. 开发文档
   - 实现细节
   - 接口说明
   - 使用示例
   - 注意事项

## 7. 示例

参考现有协议实现：
- HTTP协议
- Redis协议
- MySQL协议
- PostgreSQL协议
- AMQP协议

## 8. 注意事项

1. 保持与现有协议的一致性
2. 确保代码的可维护性
3. 注意性能优化
4. 完善错误处理
5. 添加足够的注释
6. 编写完整的测试用例
7. 及时更新文档 