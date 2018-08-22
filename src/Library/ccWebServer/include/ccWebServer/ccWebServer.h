/*
 * ccWebServer.h
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#pragma once

#include <vector>
#include <string>
#include <memory>

#include "ccWebServerEventListener.h"
#include "ccWebServerPageDirectory.h"

namespace Luna {

class ccWebServer {
public:
    ccWebServer(const std::string& name, const std::string& ports, const std::string& root_path, std::shared_ptr<ccWebServerPageDirectory> page_directory = nullptr);
    virtual ~ccWebServer();

public:
    virtual bool start();
    virtual bool stop();

    virtual void set_upload_event(const std::string& path) {}

public:
    const std::string&  getName();

    bool    set_option(std::string const& name, std::string const& value);
    void    set_listener(ccWebServerEventListener* pListener);

protected:
    //  this function performs like a initiation of web page or features for web server.
    virtual void init_server();

    const std::vector<std::string>& get_options() {
        return options_;
    }

    std::shared_ptr<ccWebServerPage> find_page(const std::string& uri);
    std::shared_ptr<ccWebServerFileUploadPage> find_file_upload_page(const std::string& uri);
    std::shared_ptr<ccWebServerFileDownloadPage> find_file_downalod_page(const std::string& uri);

protected:
    std::string                 name_;
    std::string                 httpPorts_;
    std::string                 webRootPath_;
    ccWebServerEventListener*   eventListener_;
    std::vector<std::string>    options_;

    std::shared_ptr<ccWebServerPageDirectory> webpage_directory_;
};

}
