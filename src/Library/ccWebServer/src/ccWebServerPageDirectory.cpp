/*
 * ccWebServer.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include "ccWebServer/ccWebServerPageDirectory.h"
#include "ccWebServer/ccWebServer.h"

namespace Luna {

ccWebServerPageDirectory::ccWebServerPageDirectory() {
    // TODO Auto-generated constructor stub

}

ccWebServerPageDirectory::~ccWebServerPageDirectory() {
    // TODO Auto-generated destructor stub
}

void ccWebServerPageDirectory::init(ccWebServer& server) {
    for (auto it : pages_) {
        if (it.second->is_upload_page())
            server.set_upload_event(it.second->get_path());
    }
}

bool ccWebServerPageDirectory::register_page(std::shared_ptr<ccWebServerPage> page) {
    if (page == nullptr)
        return false;

    auto item = pages_.find(page->get_path());

    if (item != pages_.end())
        return false;

    pages_[page->get_path()] = page;

    return true;
}

bool ccWebServerPageDirectory::unregister_page(const std::string& path) {
    auto item = pages_.find(path);

    if (item == pages_.end())
        return false;

    pages_.erase(path);

    return true;
}

bool ccWebServerPageDirectory::process_request(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response) {
    return false;
}

std::shared_ptr<ccWebServerPage> ccWebServerPageDirectory::find_page(const std::string& uri) {
    auto item = pages_.find(uri);

    if (item == pages_.end())
        return nullptr;

    return item->second;
}

std::shared_ptr<ccWebServerFileUploadPage> ccWebServerPageDirectory::find_file_upload_page(const std::string& uri) {
    auto item = pages_.find(uri);

    if (item == pages_.end())
        return nullptr;

    return  std::dynamic_pointer_cast<ccWebServerFileUploadPage>(item->second);
}

std::shared_ptr<ccWebServerFileDownloadPage> ccWebServerPageDirectory::find_file_downalod_page(const std::string& uri) {
    auto item = pages_.find(uri);

    if (item == pages_.end())
        return nullptr;

    return std::dynamic_pointer_cast<ccWebServerFileDownloadPage>(item->second);
}

}
