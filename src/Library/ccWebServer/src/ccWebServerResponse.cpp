/*
 * ccWebServerResponse.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include <stdio.h>

#include "ccCore/ccString.h"

#include "ccWebServerResponse.h"

namespace Luna {

ccWebServerResponse::ccWebServerResponse() :
    _uStatudCode(100),
    _bStatusSet(false),
    _bContentTypeSet(false),
    _bContentLengthSet(false),
    _bHeadersSent(false)
{
    // TODO Auto-generated constructor stub

}

ccWebServerResponse::~ccWebServerResponse()
{
    // TODO Auto-generated destructor stub
}

void ccWebServerResponse::status(unsigned int code, const std::string& strStatusText)
{
    if (_bStatusSet)
        return;

    if (code == 200)
        doPrintf("HTTP/1.1 %d OK\r\n", code);
    else
    {
        if (strStatusText.length() == 0)
            doPrintf("HTTP/1.1 %d\r\n", code);
        else
            doPrintf("HTTP/1.1 %d %s\r\n", code, strStatusText.c_str());
    }

    _bStatusSet = true;
}

void ccWebServerResponse::status(unsigned int code, const std::string& strStatusText, const std::string& strExtInfo)
{
    if (_bStatusSet)
        return;

    if (code == 200 || strStatusText.length() == 0 || strExtInfo.length() == 0)
    {
        status(code, strStatusText);
        return;
    }

    doPrintf(
        "HTTP/1.1 %d %s\r\n"
        "%s\r\n",
        code,
        strStatusText.c_str(),
        strExtInfo.c_str());

    _bStatusSet = true;
}

void ccWebServerResponse::contentType(const std::string& type, bool bInsertSeparator)
{
    contentType(type, 0, bInsertSeparator);
}

void ccWebServerResponse::contentType(const std::string& type, size_t size, bool bInsertSeparator)
{
    if (_bContentTypeSet)
        return;

    std::string strHeaderInfo;
    std::string strContentType = type;

    if (strContentType.length() == 0)
        strContentType = "text/plain";

    if (!_bStatusSet)
    {
        ccString::format(
            strHeaderInfo,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %d\r\n"        // Always set Content-Length
            "Connection: close\r\n",
            strContentType.c_str(),
            size);
    }
    else
    {
        ccString::format(
            strHeaderInfo,
            "Content-Type: %s\r\n"
            "Content-Length: %d\r\n"        // Always set Content-Length
            "Connection: close\r\n",
            strContentType.c_str(),
            size);
    }

    if (bInsertSeparator)
        strHeaderInfo += "\r\n";

    doWriteContentToConnector(strHeaderInfo.c_str(), strHeaderInfo.length());

    _bContentTypeSet = true;
}

void ccWebServerResponse::headerInfo(const std::string& type, size_t size, std::string& strExtInfo, bool bInsertSeparator)
{
    if (_bContentTypeSet)
        return;

    std::string strContentType = type;

    if (strContentType.length() == 0)
        strContentType = "text/plain";

    std::string strHeaderInfo;

    if (!_bStatusSet)
    {
        ccString::format(
            strHeaderInfo,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %d\r\n"        // Always set Content-Length
            "Connection: close\r\n"
            "%s\r\n",
            strContentType.c_str(),
            size,
            strExtInfo.c_str());
    }
    else
    {
        ccString::format(
            strHeaderInfo,
            "Content-Type: %s\r\n"
            "Content-Length: %d\r\n"        // Always set Content-Length
            "Connection: close\r\n"
            "%s\r\n",
            strContentType.c_str(),
            size,
            strExtInfo.c_str());
    }

    if (bInsertSeparator)
        strHeaderInfo += "\r\n";

    doWriteContentToConnector(strHeaderInfo.c_str(), strHeaderInfo.length());

    _bContentTypeSet = true;
}


size_t ccWebServerResponse::printf(const char *fmt, ...)
{
    doCheckHeadersSent();

    va_list ap;
    va_start(ap, fmt);

    int result = vprintf(fmt,ap);
    va_end(ap);

    return result;
}

size_t ccWebServerResponse::vprintf(const char *fmt, va_list ap)
{
    char buf[kMaxBufferSize];

    int nCalculatedSize = 0;

    if (kMaxBufferSize == 0)
        return 0;

#ifdef  WIN32
    nCalculatedSize = vsnprintf_s(buf, kMaxBufferSize, fmt, ap);
#else
    nCalculatedSize = vsnprintf(buf, kMaxBufferSize, fmt, ap);
#endif

    if (nCalculatedSize < 0)
    {
        printf("vsnprintf error");
        nCalculatedSize = 0;
    }
    else
    {
        if (nCalculatedSize >= (int) kMaxBufferSize)
        {
            printf("truncating vsnprintf buffer: [%.*s]",  nCalculatedSize > 200 ? 200 : nCalculatedSize, buf);
            nCalculatedSize = (int) kMaxBufferSize - 1;
        }
    }

    buf[nCalculatedSize] = '\0';

    return doWriteContentToConnector((const char*)buf, nCalculatedSize);
}

size_t ccWebServerResponse::write(const std::string& buf)
{
    doCheckHeadersSent();

    return doWriteContentToConnector(buf.c_str(), buf.length());
}

size_t ccWebServerResponse::write(const char* strBuf, size_t size)
{
    return doWriteContentToConnector(strBuf, size);
}

size_t ccWebServerResponse::write(std::istream & is)
{
    size_t  uSentSize = 0;
    char    buf[kMaxBufferSize];

    while (is)
    {
        is.read(&buf[0], kMaxBufferSize);

        uSentSize += doWriteContentToConnector((const char*)&buf[0], (size_t)is.gcount());
    }

    return uSentSize;
}

bool ccWebServerResponse::notFoundFile(const std::string& strURI)
{
    std::string strContent;

    ccString::format(
        strContent,
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

    status(404, "Not Found");
    contentType("text/html", strContent.length());

    doWriteContentToConnector(strContent.c_str(), strContent.length());

    return true;
}


void ccWebServerResponse::doCheckHeadersSent()
{
    if (_bHeadersSent)
        return;

    if (!_bContentTypeSet)
        contentType("text/plain");

    _bHeadersSent = true;
}

size_t ccWebServerResponse::doPrintf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    size_t result = vprintf(fmt, ap);

    va_end(ap);


    return result;
}

}
