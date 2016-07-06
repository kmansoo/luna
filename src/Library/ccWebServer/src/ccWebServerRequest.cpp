/*
 * ccWebServerRequest.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include "../ccWebServerRequest.h"

namespace Luna {

ccWebServerRequest::ccWebServerRequest()
{
    // TODO Auto-generated constructor stub

}

ccWebServerRequest::~ccWebServerRequest()
{
    // TODO Auto-generated destructor stub
}

ccWebServerRequest::HttpMethod  ccWebServerRequest::getMethod() const
{
    if (_strMethod == "GET")        return HttpMethod_Get;
    if (_strMethod == "HEAD")       return HttpMethod_Head;
    if (_strMethod == "POST")       return HttpMethod_Post;
    if (_strMethod == "PUT")        return HttpMethod_Put;
    if (_strMethod == "DELETE")     return HttpMethod_Delete;
    if (_strMethod == "TRACE")      return HttpMethod_Trace;
    if (_strMethod == "CONNECT")    return HttpMethod_Connect;

    return HttpMethod_Unknown;
}

const std::string& ccWebServerRequest::getMethod_c() const
{
    return _strMethod;
}

const std::string& ccWebServerRequest::getURI() const
{
    return _strUri;
}

std::string ccWebServerRequest::getQueryString()
{
    return _strNullData;
}

std::string ccWebServerRequest::getPath()
{
    return _strNullData;
}

std::string ccWebServerRequest::getResource()
{
    return _strNullData;
}

bool ccWebServerRequest::hasVar(const std::string& name) const
{
    return false;
}

std::string ccWebServerRequest::getVar(const std::string& name)
{
    return _strNullData;
}

std::string ccWebServerRequest::getHeader(const std::string& name)
{
    return _strNullData;
}

std::string ccWebServerRequest::getContentType()
{
    return _strNullData;
}

unsigned long ccWebServerRequest::getContentLength()
{
    return 0;
}

// Retrieve info for the server interface that was connected with the remote device.
long ccWebServerRequest::getServerIp() const
{
    return 0;
}

unsigned short  ccWebServerRequest::getServerPort() const
{
    return 0;
}

long ccWebServerRequest::getRemoteIp() const
{
    return 0;
}

long ccWebServerRequest::getContentBody(std::string& strBody)
{
    return 0;
}

long ccWebServerRequest::getContentBody(char* pBuf, std::size_t size)
{
    return 0;
}

}
