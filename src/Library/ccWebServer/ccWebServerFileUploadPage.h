/*
* ccWebServerFileUploadPage.h
*
*  Created on: 2016. 8. 12.
*      Author: Mansoo Kim
*/

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERFILEUPLOADLOADPAGE_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERFILEUPLOADLOADPAGE_H_

#include <mutex>
#include <unordered_map>

#include "ccWebServerPage.h"

#include <fstream>

namespace Luna {

class ccWebServerFileUploadPage;

class ccWebServerFileUploadAgent {
public:
    ccWebServerFileUploadAgent(std::uint32_t id, std::string local_file_path, ccWebServerFileUploadPage* parent);
    ~ccWebServerFileUploadAgent();

public:
    std::uint32_t   get_id() { return id_; }
    ccWebServerFileUploadPage* get_parent() { return parent_; }

    bool    on_file_info(const std::string& file_name, size_t file_size);
    bool    on_receive_data(const char* data, size_t size);
    bool    on_finish_upload();

protected:
    std::uint32_t   id_;
    ccWebServerFileUploadPage*  parent_;

    std::string     local_file_path_;

    std::string     upload_file_name_;
    std::uint32_t   upload_file_length_;
    std::uint32_t   received_data_length_;

    std::ofstream   upload_file_;
};

class ccWebServerFileUploadPage : public ccWebServerPage {
public:
    ccWebServerFileUploadPage(const std::string& path, bool single_agent = true);
    ccWebServerFileUploadPage(const std::string& path, const std::string& local_file_path, bool single_agent = true);

    virtual ~ccWebServerFileUploadPage() {}

public:
    virtual bool    is_upload_page() { return true; }

public:
    ccWebServerFileUploadAgent* create_file_upload_agent();
    void                        destroy_file_upload_agent(ccWebServerFileUploadAgent* agent);

private:
    bool single_agent_;

    std::mutex mtx_;
    std::uint32_t generate_id_;
    std::string local_file_path_;

    std::unordered_map<std::uint32_t, std::shared_ptr<ccWebServerFileUploadAgent>> agents_;
};

}

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERFILEUPLOADLOADPAGE_H_ */
