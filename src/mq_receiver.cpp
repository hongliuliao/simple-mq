/*
 * mq_receiver.cpp
 *
 *  Created on: Nov 30, 2014
 *      Author: liao
 */
#include <map>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cerrno>

#include "json/json.h"

#include "simple_log.h"
#include "http_parser.h"
#include "http_server.h"

struct ReceiveServer {
    time_t aof_last_shift_time;
    time_t aof_shift_interval;
    std::string log_path;
};

ReceiveServer server;

void shift_file_if_need(std::string aof_file_path) {
    std::fstream fs(aof_file_path.c_str(), std::fstream::out | std::fstream::in);
    if(!fs) {
        return;
    }

    time_t now = time(NULL);
    if(now - server.aof_last_shift_time > server.aof_shift_interval) {
        int format_time_len = 15;
        int new_file_name_len = strlen(aof_file_path.c_str()) + format_time_len+ 2; // "." & "\0"
        char format_time_str[format_time_len];
        char new_name[new_file_name_len];
        bzero(format_time_str, format_time_len);
        bzero(new_name, new_file_name_len);

        struct tm *time_info = localtime(&now);
        strftime(format_time_str, format_time_len, "%Y%m%d%H%M%S", time_info);

        snprintf(new_name, new_file_name_len, "%s.%s", aof_file_path.c_str(), format_time_str);
        LOG_INFO("start shift aof file which aof_last_shift_time: %ld, aof_shift_interval:%d, new_file:%s", server.aof_last_shift_time, server.aof_shift_interval, new_name);
        if (rename(aof_file_path.c_str(), new_name) == 0) {
            server.aof_last_shift_time = now;
        } else {
            LOG_WARN("rename aof fail : %s", strerror(errno));
        }
    }
}

Response receive_msg(Request &req) {
    std::map<std::string, std::string> params = req.body.get_params();
    std::string queue = req.get_param("queue");
    if(params.size() == 0 || queue.empty()) {
        Json::Value root;
        root["code"] = 1;
        root["msg"] = "queue is requried!";
        return Response(STATUS_OK, root);
    }

    std::stringstream body_content;
    int i = 0;
    for (std::map<std::string, std::string>::iterator it = params.begin(); it != params.end(); ++it) {
        if(i++ != 0) {
            body_content << "&";
        }
        body_content << it->first << "=" << it->second;
    }
    LOG_DEBUG("build body_content success : %s", body_content.str().c_str());

    std::string aof_file_path = server.log_path + queue + ".log";

    shift_file_if_need(aof_file_path);
    std::fstream fs(aof_file_path.c_str(), std::fstream::out | std::fstream::app);
    if(!fs) {
        LOG_WARN("Opening aof file (%s) failed: %s", aof_file_path.c_str(), strerror(errno));
        Json::Value root;
        root["code"] = 2;
        root["msg"] = "Opening aof file failed!";
        return Response(STATUS_OK, root);
    }

    fs << body_content.str() << "\n";
    fs.close();

    Json::Value root;
    root["code"] = 0;
    root["msg"] = "success";
    return Response(STATUS_OK, root);
};

int main() {
    time_t now = time(NULL);
    server.aof_last_shift_time = now;
    server.aof_shift_interval = 3600;
    server.log_path = "/tmp/";

    HttpServer http_server;
    http_server.add_mapping("/msg", receive_msg);
    http_server.start(3493);
    return 0;
}
