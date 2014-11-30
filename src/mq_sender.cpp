/*
 * mq_sender.cpp
 *
 *  Created on: Nov 30, 2014
 *      Author: liao
 */
#include "curl/curl.h"
#include "simple_log.h"
#include "flow_handler.h"
#include "file_agent.h"
#include "curl_utils.h"

class LogSendHandler : public LineFlowHandler {

public:
    int handle_lines(std::vector<std::string> lines) {
        for(unsigned i = 0;i<lines.size();++i) {
            std::string result;
            CURLcode code = mq::CurlUtils::post_body("http://localhost:3495/msg_consume", lines[i], result);
            if(code == 0) {
                LOG_INFO("send msg success! ret:%s", result.c_str())
            } else {
                LOG_INFO("send msg fail which ret:%d", code);
            }
        }
        return 0;
    }
};

int main() {
    LogSendHandler h;
    FileAgent fa;
    fa.set_flow_handler(h);
    fa.start("/tmp/test.log");
    return 0;
}


