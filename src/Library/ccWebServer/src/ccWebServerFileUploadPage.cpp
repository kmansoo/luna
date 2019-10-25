/*
* ccWebServerFileUploadPage.cpp
*
*  Created on: 2016. 8. 12.
*      Author: Mansoo Kim
*/

#include "ccWebServer/ccWebServerFileUploadPage.h"

namespace Luna {

// class ccWebServerFileUploadAgent
ccWebServerFileUploadAgent::ccWebServerFileUploadAgent(
    std::uint32_t id,
    std::string local_file_path,
    ccWebServerFileUploadPage* parent)
    : id_(id), parent_(parent), local_file_path_(local_file_path) {
    upload_file_length_ = 0;
    received_data_length_ = 0;
}

ccWebServerFileUploadAgent::~ccWebServerFileUploadAgent() {
    on_finish_upload();
}

bool ccWebServerFileUploadAgent::on_file_info(const std::string& file_name, size_t file_size) {
     std::cout << "ccWebServerFileUploadAgent::on_file_info(), file_name:" << file_name << std::endl;

    std::string open_file_name = file_name;

    if (open_file_name.length() == 0) {
        open_file_name = local_file_path_;

        if (open_file_name.length() == 0)
            open_file_name = "noname_uploadfile.dat";
    }

    upload_file_.open(open_file_name, std::ios_base::binary);

    return upload_file_.is_open();
}

bool ccWebServerFileUploadAgent::on_receive_data(const char* data, size_t size) {
    if (!upload_file_.is_open())
        return false;

    upload_file_.write(data, size);

    return true;
}

bool ccWebServerFileUploadAgent::on_finish_upload() {
    if (!upload_file_.is_open())
        return false;

    upload_file_.close();

    std::cout << "ccWebServerFileUploadAgent::on_finish_upload(), done" << std::endl;

    return true;
}

// class ccWebServerFileUploadPage
ccWebServerFileUploadPage::ccWebServerFileUploadPage(const std::string& path, bool single_agent)
    : ccWebServerPage(path), generate_id_(0x00) {
}

ccWebServerFileUploadPage::ccWebServerFileUploadPage(
    const std::string& path,
    const std::string& local_file_path,
    bool single_agent)
    : ccWebServerPage(path),
    generate_id_(0x00),
    local_file_path_(local_file_path) {
}

ccWebServerFileUploadAgent* ccWebServerFileUploadPage::create_file_upload_agent() {
    if (single_agent_ && agents_.size() != 0)
        return nullptr;

    mtx_.lock();
    generate_id_++;
    mtx_.unlock();

    std::shared_ptr<ccWebServerFileUploadAgent> new_agent =
        std::make_shared<ccWebServerFileUploadAgent>(generate_id_, local_file_path_,
                                                     this);

    agents_[generate_id_] = new_agent;

    return new_agent.get();
}

void ccWebServerFileUploadPage::destroy_file_upload_agent(ccWebServerFileUploadAgent* agent) {
    if (agent)
        agents_.erase(agent->get_id());
}

}
