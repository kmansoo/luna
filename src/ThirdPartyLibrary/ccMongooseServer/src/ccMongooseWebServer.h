/*
 * ccMongooseWebServer.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#pragma once

#include <thread>

#include "mongoose/mongoose.h"

#include "ccWebServer/ccWebServer.h"

namespace Luna {

class ccMongooseWebServer : public ccWebServer
{
public:
    ccMongooseWebServer(const std::string& name, const std::string& ports, const std::string& root_path, std::shared_ptr<ccWebServerPageDirectory> page_directory = nullptr);
    virtual ~ccMongooseWebServer();

public:
    virtual bool start();
    virtual bool stop();
    virtual void set_upload_event(const std::string& path);

private:
    static void ev_handler(struct mg_connection *nc, int ev, void *p);
    static void ev_handler_upload(struct mg_connection *nc, int ev, void *p);

private:
    void    do_run_thread();

private:
    bool                    isStopThread_;

    std::string             last_multipart_request_uri_;
    std::thread*            pollThread_;
    struct mg_mgr*          mgr_;
    struct mg_connection*   con_;
};

}
