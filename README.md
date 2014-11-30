simple_mq
=========

这是一个基于simple_server + simple_flow实现的mq,具体有如下特点
1. 使用推的模式
2. 使用http作为接口
3. 消息持久化在文件中

## 使用指南
* make && make test
* 启动mq_receiver,用于消息接收
* 启动mq_sender用于消息发送
