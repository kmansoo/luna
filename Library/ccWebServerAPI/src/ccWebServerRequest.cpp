/*
 * ccWebServerRequest.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include "../ccWebServerRequest.h"

ccWebServerRequest::ccWebServerRequest()
{
    // TODO Auto-generated constructor stub

}

ccWebServerRequest::~ccWebServerRequest()
{
    // TODO Auto-generated destructor stub
}

ccWebServerRequest::HttpMethod  ccWebServerRequest::GetMethod() const
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

const string& ccWebServerRequest::GetMethod_c() const
{
    return _strMethod;
}

const string& ccWebServerRequest::GetURI() const
{
    return _strUri;
}

const string& ccWebServerRequest::GetQueryString() const
{
    return _strNullData;
}

string ccWebServerRequest::GetPath() const
{
    return _strNullData;
}

string ccWebServerRequest::GetResource() const
{
    return _strNullData;
}

bool ccWebServerRequest::HasVar(const string& name) const
{
    return false;
}


string ccWebServerRequest::GetVar(const string& name) const
{
    return _strNullData;
}

string ccWebServerRequest::GetHeader(const string& name) const
{
    return _strNullData;
}

string ccWebServerRequest::GetContentType() const
{
    return _strNullData;
}

unsigned long ccWebServerRequest::GetContentLength() const
{
    return 0;
}


// Retrieve info for the server interface that was connected with the remote device.
long ccWebServerRequest::GetServerIp() const
{
    return 0;
}

unsigned short  ccWebServerRequest::GetServerPort() const
{
    return 0;
}

long ccWebServerRequest::GetRemoteIp() const
{
    return 0;
}

const char* ccWebServerRequest::GetPostData(unsigned long* size) const
{
    if (size != 0)  // 0 means NULL
       *size = 0;

    return _strNullData.c_str();
}
