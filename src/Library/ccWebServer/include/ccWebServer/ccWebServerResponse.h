/*
 * ccWebServerResponse.h
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#pragma once

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

public:
    virtual void    send_status(unsigned int code, const std::string& strStatusText, bool bNoContent = false);
    virtual void    send_status(unsigned int code, const std::string& strStatusText, const std::string& strExtInfo);

    virtual void    send_content_type(const std::string& type, bool bInsertSeparator = true);
    virtual void    send_content_type(const std::string& type, size_t size, bool bInsertSeparator = true);

    virtual void    header_info(const std::string& type, size_t size, std::string& strExtInfo, bool bInsertSeparator = true);

    virtual size_t  send_formatted_content(const char *fmt, ...);   // printf
    virtual size_t  send_formatted_content_ex(const char *fmt, va_list ap); //  vprintf
    virtual size_t  send_content(const std::string& buf);
    virtual size_t  send_content(const char* strBuf, size_t size);
    virtual size_t  send_content(std::istream& is);
    virtual size_t  send_file(const std::string& path, const std::string& media_type) { return 0; }

    virtual void    close_without_content();

    virtual bool    not_found_file(const std::string& strURI);

protected:
    virtual size_t write_content_to_connector(const char* strBuf, size_t size) { return 0; }
    virtual void   check_header_sent();

protected:
    size_t  send_formatted_content_impl(const char *fmt, ...);

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
