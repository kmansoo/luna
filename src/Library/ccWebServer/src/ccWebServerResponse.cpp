/*
 * ccWebServerResponse.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include <stdio.h>

#include "ccCore/ccString.h"

#include "ccWebServer/ccWebServerResponse.h"

namespace Luna {

ccWebServerResponse::ccWebServerResponse() :
    status_code_(100),
    is_status_set_(false),
    is_content_type_set_(false),
    is_content_length_set_(false),
    is_haders_sent_(false) {
    // TODO Auto-generated constructor stub

}

ccWebServerResponse::~ccWebServerResponse() {
    // TODO Auto-generated destructor stub
}

void ccWebServerResponse::send_status(unsigned int code, const std::string& strStatusText, bool bNoContent) {
    if (is_status_set_)
        return;

    if (code == 200)
        send_formatted_content_impl("HTTP/1.1 %d OK\r\n", code);
    else {
        if (strStatusText.length() == 0)
            send_formatted_content_impl("HTTP/1.1 %d\r\n", code);
        else
            send_formatted_content_impl("HTTP/1.1 %d %s\r\n", code, strStatusText.c_str());
    }

    is_status_set_ = true;

    if (bNoContent)
        close_without_content();
}

void ccWebServerResponse::send_status(unsigned int code, const std::string& strStatusText, const std::string& strExtInfo) {
    if (is_status_set_)
        return;

    if (code == 200 || strStatusText.length() == 0 || strExtInfo.length() == 0) {
        send_status(code, strStatusText, true);
        return;
    }

    send_formatted_content_impl(
        "HTTP/1.1 %d %s\r\n"
        "%s\r\n",
        code,
        strStatusText.c_str(),
        strExtInfo.c_str());

    is_status_set_ = true;
}

void ccWebServerResponse::send_content_type(const std::string& type, bool bInsertSeparator) {
    send_content_type(type, 0, bInsertSeparator);
}

void ccWebServerResponse::send_content_type(const std::string& type, size_t size, bool bInsertSeparator) {
    if (is_content_type_set_)
        return;

    std::string header_info;
    std::string content_type = type;

    if (content_type.length() == 0)
        content_type = "text/plain";

    if (!is_status_set_) {
        ccString::format(
            header_info,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %d\r\n"        // Always set Content-Length
            "Connection: close\r\n",
            content_type.c_str(),
            size);
    } else {
        ccString::format(
            header_info,
            "Content-Type: %s\r\n"
            "Content-Length: %d\r\n"        // Always set Content-Length
            "Connection: close\r\n",
            content_type.c_str(),
            size);
    }

    if (bInsertSeparator)
        header_info += "\r\n";

    write_content_to_connector(header_info.c_str(), header_info.length());

    is_content_type_set_ = true;
}

void ccWebServerResponse::header_info(const std::string& type, size_t size, std::string& strExtInfo, bool bInsertSeparator) {
    if (is_content_type_set_)
        return;

    std::string content_type = type;

    if (content_type.length() == 0)
        content_type = "text/plain";

    std::string header_info;

    if (!is_status_set_) {
        ccString::format(
            header_info,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %d\r\n"        // Always set Content-Length
            "Connection: close\r\n"
            "%s\r\n",
            content_type.c_str(),
            size,
            strExtInfo.c_str());
    } else {
        ccString::format(
            header_info,
            "Content-Type: %s\r\n"
            "Content-Length: %d\r\n"        // Always set Content-Length
            "Connection: close\r\n"
            "%s\r\n",
            content_type.c_str(),
            size,
            strExtInfo.c_str());
    }

    if (bInsertSeparator)
        header_info += "\r\n";

    write_content_to_connector(header_info.c_str(), header_info.length());

    is_content_type_set_ = true;
}


size_t ccWebServerResponse::send_formatted_content(const char *fmt, ...) {
    check_header_sent();

    va_list ap;
    va_start(ap, fmt);

    int result = send_formatted_content_ex(fmt, ap);
    va_end(ap);

    return result;
}

size_t ccWebServerResponse::send_formatted_content_ex(const char *fmt, va_list ap) {
    char buf[kMaxBufferSize];

    int calculated_size = 0;

    if (kMaxBufferSize == 0)
        return 0;

#ifdef  WIN32
    calculated_size = vsnprintf_s(buf, kMaxBufferSize, fmt, ap);
#else
    calculated_size = vsnprintf(buf, kMaxBufferSize, fmt, ap);
#endif

    if (calculated_size < 0) {
        printf("vsnprintf error");
        calculated_size = 0;
    } else {
        if (calculated_size >= (int)kMaxBufferSize) {
            printf("truncating vsnprintf buffer: [%.*s]", calculated_size > 200 ? 200 : calculated_size, buf);
            calculated_size = (int)kMaxBufferSize - 1;
        }
    }

    buf[calculated_size] = '\0';

    return write_content_to_connector((const char*)buf, calculated_size);
}

size_t ccWebServerResponse::send_content(const std::string& buf) {
    check_header_sent();

    return write_content_to_connector(buf.c_str(), buf.length());
}

size_t ccWebServerResponse::send_content(const char* strBuf, size_t size) {
    return write_content_to_connector(strBuf, size);
}

size_t ccWebServerResponse::send_content(std::istream & is) {
    size_t  sent_size = 0;
    char    buf[kMaxBufferSize];

    while (is) {
        is.read(&buf[0], kMaxBufferSize);

        sent_size += write_content_to_connector((const char*)&buf[0], (size_t)is.gcount());
    }

    return sent_size;
}

void ccWebServerResponse::close_without_content() {
    if (!is_status_set_)
        return;

    if (!is_content_type_set_)
        send_content_type(std::string(""));
}

bool ccWebServerResponse::not_found_file(const std::string& strURI) {
    std::string content;

    ccString::format(
        content,
        "<!DOCTYPE html>\r\n"
        "<html lang=en>\r\n"
        "<head>\r\n"
        "  <meta charset=utf-8>\r\n"
        "  <meta name=viewport content=\"initial-scale=1, minimum-scale=1, width=device-width\">\r\n"
        "  <title>Error 404 (Not Found)!!1</title>\r\n"
        "</head>\r\n"
        "<body>\r\n"
        "  <p><b>404.</b> <ins>That's an error.</ins></p>\r\n"
        "  <p>The requested URL <code>%s</code> was not found on this server.</p>\r\n"
        "</body>\r\n"
        "</html>\r\n",
        strURI.c_str());

    send_status(404, "Not Found", false);
    send_content_type("text/html", content.length());

    write_content_to_connector(content.c_str(), content.length());

    return true;
}

void ccWebServerResponse::check_header_sent() {
    if (is_haders_sent_)
        return;

    if (!is_content_type_set_)
        send_content_type("text/plain");

    is_haders_sent_ = true;
}

size_t ccWebServerResponse::send_formatted_content_impl(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    size_t result = send_formatted_content_ex(fmt, ap);

    va_end(ap);

    return result;
}

}
