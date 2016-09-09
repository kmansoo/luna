/*
 * ccWebServerResponse.h
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERRESPONSE_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERRESPONSE_H_

#include <stdarg.h>

#include <string>
#include <iostream>

namespace Luna {

class ccWebServerResponse {
public:
    ccWebServerResponse();
    virtual ~ccWebServerResponse();

public:
    enum ConstValue {
        kMaxBufferSize = 8192
    };

protected:
    virtual size_t write_content_to_connector(const char* strBuf, size_t size) { return 0; }

protected:
    void    check_header_sent();
    size_t  send_formatted_content_impl(const char *fmt, ...);

public:
    void    send_status(unsigned int code, const std::string& strStatusText, bool bNoContent = false);
    void    send_status(unsigned int code, const std::string& strStatusText, const std::string& strExtInfo);

    void    send_content_type(const std::string& type, bool bInsertSeparator = true);
    void    send_content_type(const std::string& type, size_t size, bool bInsertSeparator = true);

    void    header_info(const std::string& type, size_t size, std::string& strExtInfo, bool bInsertSeparator = true);

    size_t  send_formatted_content(const char *fmt, ...);   // printf
    size_t  send_formatted_content_ex(const char *fmt, va_list ap); //  vprintf
    size_t  send_content(const std::string& buf);
    size_t  send_content(const char* strBuf, size_t size);
    size_t  send_content(std::istream& is);

    void    close_without_content();

    bool    not_found_file(const std::string& strURI);

protected:
    unsigned int    status_code_;
    std::string     status_text_;
    std::string     ext_info_;

    bool    is_status_set_;
    bool    is_content_type_set_;
    bool    is_content_length_set_;
    bool    is_haders_sent_;

};

}

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERRESPONSE_H_ */
