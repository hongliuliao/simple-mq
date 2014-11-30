/*
 * mq_consumer.cpp
 *
 *  Created on: Nov 30, 2014
 *      Author: liao
 */
#include <sstream>
#include <cstdlib>
#include "simple_log.h"
#include "http_server.h"

Response msg_consume(Request &request) {
    std::string name = request.get_param("name");
    std::string queue = request.get_param("queue");

    LOG_DEBUG("msg_consume which name:%s, queue:%s", name.c_str(), queue.c_str());
    Json::Value root;
    root["code"] = 0;
    root["msg"] = "login success!";
    return Response(STATUS_OK, root);
}

int main() {
    HttpServer http_server;

    http_server.add_mapping("/msg_consume", msg_consume, POST_METHOD);

    http_server.start(3495);
    return 0;
}
