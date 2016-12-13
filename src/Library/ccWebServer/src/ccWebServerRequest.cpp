/*
 * ccWebServerRequest.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include "ccWebServer/ccWebServerRequest.h"

namespace Luna {

ccWebServerRequest::ccWebServerRequest() {
    // TODO Auto-generated constructor stub

}

ccWebServerRequest::~ccWebServerRequest() {
    // TODO Auto-generated destructor stub
}

ccWebServerRequest::HttpMethod  ccWebServerRequest::get_method() const {
    if (method_ == "GET")        return HttpMethod_Get;
    if (method_ == "HEAD")       return HttpMethod_Head;
    if (method_ == "POST")       return HttpMethod_Post;
    if (method_ == "PUT")        return HttpMethod_Put;
    if (method_ == "DELETE")     return HttpMethod_Delete;
    if (method_ == "TRACE")      return HttpMethod_Trace;
    if (method_ == "CONNECT")    return HttpMethod_Connect;

    return HttpMethod_Unknown;
}

const std::string& ccWebServerRequest::get_method_c() const {
    return method_;
}

const std::string& ccWebServerRequest::geturi_() const {
    return uri_;
}

std::string ccWebServerRequest::get_query_string() {
    return blank_string_data_;
}

std::string ccWebServerRequest::get_path() {
    return blank_string_data_;
}

std::string ccWebServerRequest::get_resource() {
    return blank_string_data_;
}

bool ccWebServerRequest::has_variable(const std::string& name) const {
    return false;
}

std::string ccWebServerRequest::get_variable(const std::string& name) {
    return blank_string_data_;
}

std::string ccWebServerRequest::get_header(const std::string& name) {
    return blank_string_data_;
}

std::string ccWebServerRequest::get_content_type() {
    return blank_string_data_;
}

unsigned long ccWebServerRequest::get_content_length() {
    return 0;
}

// Retrieve info for the server interface that was connected with the remote device.
long ccWebServerRequest::get_server_ip() const {
    return 0;
}

unsigned short  ccWebServerRequest::get_server_port() const {
    return 0;
}

long ccWebServerRequest::get_remote_ip() const {
    return 0;
}

long ccWebServerRequest::get_content_body(std::string& strBody) {
    return 0;
}

long ccWebServerRequest::get_content_body(char* pBuf, std::size_t size) {
    return 0;
}

}
