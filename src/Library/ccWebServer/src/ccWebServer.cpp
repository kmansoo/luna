/*
 * ccWebServer.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include "ccWebServer/ccWebServer.h"

namespace Luna {

ccWebServer::ccWebServer(
    const std::string& name,
    const std::string& ports,
    const std::string& root_path,
    std::shared_ptr<ccWebServerPageDirectory> page_directory)
    : name_(name),
    httpPorts_(ports),
    webRootPath_(root_path),
    eventListener_(NULL),
    webpage_directory_(page_directory) {

}

ccWebServer::~ccWebServer() {
}

bool ccWebServer::start() {
    return false;
}

bool ccWebServer::stop() {
    return false;
}

void ccWebServer::init_server() {
    if (webpage_directory_)
        webpage_directory_->init(*this);
}

const std::string& ccWebServer::get_name() {
    return name_;
}

bool ccWebServer::set_option(std::string const& name,
                             std::string const& value) {
    options_.push_back(name);
    options_.push_back(value);

    return true;
}

void ccWebServer::set_listener(ccWebServerEventListener* listener) {
    eventListener_ = listener;
}

std::shared_ptr<ccWebServerPage> ccWebServer::find_page(const std::string& uri) {
    if (!webpage_directory_)
        return nullptr;

    return webpage_directory_->find_page(uri);
}

std::shared_ptr<ccWebServerFileUploadPage> ccWebServer::find_file_upload_page(const std::string& uri) {
    if (!webpage_directory_)
        return nullptr;

    return webpage_directory_->find_file_upload_page(uri);
}

std::shared_ptr<ccWebServerFileDownloadPage>
ccWebServer::find_file_downalod_page(const std::string& uri) {

    if (!webpage_directory_)
        return nullptr;

    return webpage_directory_->find_file_downalod_page(uri);
}

}
