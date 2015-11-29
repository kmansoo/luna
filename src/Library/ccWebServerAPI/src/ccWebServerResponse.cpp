/*
 * ccWebServerResponse.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include <stdio.h>

#include "ccCoreAPI/ccString.h"

#include "ccWebServerResponse.h"


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

void ccWebServerResponse::Status(unsigned int code, const string& strStatusText)
{
    if (_bStatusSet)
        return;

    if (code == 200)
        DoPrintf("HTTP/1.1 %d OK\r\n", code);
    else
    {
        if (strStatusText.length() == 0)
            DoPrintf("HTTP/1.1 %d\r\n", code);
        else
            DoPrintf("HTTP/1.1 %d %s\r\n", code, strStatusText.c_str());
    }

    _bStatusSet = true;
}

void ccWebServerResponse::Status(unsigned int code, const string& strStatusText, const string& strExtInfo)
{
    if (_bStatusSet)
        return;

    if (code == 200 || strStatusText.length() == 0 || strExtInfo.length() == 0)
    {
        Status(code, strStatusText);
        return;
    }

    DoPrintf(
        "HTTP/1.1 %d %s\r\n"
        "%s\r\n",
        code,
        strStatusText.c_str(),
        strExtInfo.c_str());

    _bStatusSet = true;
}

void ccWebServerResponse::ContentType(const string& type, bool bInsertSeparator)
{
    ContentType(type, 0, bInsertSeparator);
}

void ccWebServerResponse::ContentType(const string& type, size_t size, bool bInsertSeparator)
{
    if (_bContentTypeSet)
        return;

    string strHeaderInfo;
    string strContentType = type;

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

    DoWriteContentToConnector(strHeaderInfo.c_str(), strHeaderInfo.length());

    _bContentTypeSet = true;
}

void ccWebServerResponse::HeaderInfo(const string& type, size_t size, string& strExtInfo, bool bInsertSeparator)
{
    if (_bContentTypeSet)
        return;

    string strContentType = type;

    if (strContentType.length() == 0)
        strContentType = "text/plain";

    string strHeaderInfo;

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

    DoWriteContentToConnector(strHeaderInfo.c_str(), strHeaderInfo.length());

    _bContentTypeSet = true;
}


size_t ccWebServerResponse::Printf(const char *fmt, ...)
{
    DoCheckHeadersSent();

    va_list ap;
    va_start(ap, fmt);

    int result = VPrintf(fmt,ap);
    va_end(ap);

    return result;
}

size_t ccWebServerResponse::VPrintf(const char *fmt, va_list ap)
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

    return DoWriteContentToConnector((const char*)buf, nCalculatedSize);
}

size_t ccWebServerResponse::Write(const string& buf)
{
    DoCheckHeadersSent();

    return DoWriteContentToConnector(buf.c_str(), buf.length());
}

size_t ccWebServerResponse::Write(const char* strBuf, size_t size)
{
    return DoWriteContentToConnector(strBuf, size);
}

size_t ccWebServerResponse::Write(istream & is)
{
    size_t  uSentSize = 0;
    char    buf[kMaxBufferSize];

    while (is)
    {
        is.read(&buf[0], kMaxBufferSize);

        uSentSize += DoWriteContentToConnector((const char*)&buf[0], (size_t)is.gcount());
    }

    return uSentSize;
}

bool ccWebServerResponse::NotFoundFile(const string& strURI)
{
    string strContent;

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

    Status(404, "Not Found");
    ContentType("text/html", strContent.length());

    DoWriteContentToConnector(strContent.c_str(), strContent.length());

    return true;
}


void ccWebServerResponse::DoCheckHeadersSent()
{
    if (_bHeadersSent)
        return;

    if (!_bContentTypeSet)
        ContentType("text/plain");

    _bHeadersSent = true;
}

size_t ccWebServerResponse::DoPrintf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    size_t result = VPrintf(fmt, ap);

    va_end(ap);

    return result;
}
