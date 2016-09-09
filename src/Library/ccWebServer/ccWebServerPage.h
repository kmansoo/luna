/*
 * ccWebServerPage.h
 *
 *  Created on: 2016. 8. 12.
 *      Author: Mansoo Kim
 */

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERPAGE_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERPAGE_H_

#include <string>
#include <memory>

#include "ccWebServerRequest.h"
#include "ccWebServerResponse.h"

namespace Luna {

class ccWebServerPage {
public:
    ccWebServerPage(const std::string& path) { path_ = path; }
    ccWebServerPage(const std::string& path, const std::string& name) { path_ = path; file_name_ = name; }
    virtual ~ccWebServerPage() {}

public:
    const std::string&  get_path() const { return path_; }
    const std::string&  get_file_name() const { return file_name_; }
    const std::string&  get_mime_type() const { return mime_type_; }

public:
    virtual bool        is_upload_page() { return false; }
    virtual bool        is_download_page() { return false; }

    virtual bool        process(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response) { return false; }
    virtual const char* get_content() { return NULL; }
    virtual size_t      get_content_length() const { return 0; }

protected:
    std::string     path_;
    std::string     file_name_;
    std::string     mime_type_;
};

}

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERPAGE_H_ */
