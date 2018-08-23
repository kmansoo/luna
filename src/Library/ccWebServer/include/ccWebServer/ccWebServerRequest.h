/*
 * ccWebServerRequest.h
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#pragma once

#include <string>

namespace Luna {

class ccWebServerRequest {
public:
    ccWebServerRequest();
    virtual ~ccWebServerRequest();

public:
    enum HttpMethod {
        HttpMethod_Unknown,
        HttpMethod_Get,
        HttpMethod_Head,
        HttpMethod_Post,
        HttpMethod_Put,
        HttpMethod_Delete,
        HttpMethod_Trace,
        HttpMethod_Connect
    };

    enum CONST_VALUE {
        CV_MAXGETSIZE = 1024
    };


public:
    virtual HttpMethod          get_method() const;
    virtual const std::string&  get_method_c() const;
    virtual const std::string&  geturi_() const;

    virtual std::string         get_query_string();
    virtual std::string         get_path();
    virtual std::string         get_resource();

    virtual bool                has_variable(const std::string& name) const;
    virtual std::string         get_variable(const std::string& name);

    virtual std::string         get_header(const std::string& name);

    virtual std::string         get_content_type();
    virtual unsigned long       get_content_length();

    virtual long                get_content_body(std::string& strBody);
    virtual long                get_content_body(char* pBuf, std::size_t size);

    // Retrieve info for the server interface that was connected with the remote device.
    virtual long                get_server_ip() const;
    virtual unsigned short      get_server_port() const;
    virtual long                get_remote_ip() const;

protected:
    std::string blank_string_data_;

    std::string method_;
    std::string uri_;
};

}
