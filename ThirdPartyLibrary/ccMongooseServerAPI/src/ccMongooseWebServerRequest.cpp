/*
 * ccMongooseWebServerRequest.cpp
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#include "ccMongooseWebServerRequest.h"

ccMongooseWebServerRequest::ccMongooseWebServerRequest(struct mg_connection* con, struct http_message* http_msg)
    : _pMgConnection(con),
      _pMgHttpMessage(http_msg)
{
    // TODO Auto-generated constructor stub

}

ccMongooseWebServerRequest::~ccMongooseWebServerRequest()
{
    // TODO Auto-generated destructor stub
}


ccMongooseWebServerRequest::HttpMethod  ccMongooseWebServerRequest::GetMethod() const
{
    if (_pMgHttpMessage == NULL)
        return HttpMethod_Unknown;

    if (strcmp("GET",       _pMgHttpMessage->method.p) == 0)return HttpMethod_Get;
    if (strcmp("HEAD",      _pMgHttpMessage->method.p) == 0)return HttpMethod_Head;
    if (strcmp("POST",      _pMgHttpMessage->method.p) == 0)return HttpMethod_Post;
    if (strcmp("PUT",       _pMgHttpMessage->method.p) == 0)return HttpMethod_Put;
    if (strcmp("DELETE",    _pMgHttpMessage->method.p) == 0)return HttpMethod_Delete;
    if (strcmp("TRACE",     _pMgHttpMessage->method.p) == 0)return HttpMethod_Trace;
    if (strcmp("CONNECT",   _pMgHttpMessage->method.p) == 0)return HttpMethod_Connect;

    return HttpMethod_Unknown;
}

string ccMongooseWebServerRequest::GetMethod_c() const
{
    if (_pMgHttpMessage->method.p == NULL)
        return _strNullData;

    return _pMgHttpMessage->method.p;
}

string ccMongooseWebServerRequest::GetURI() const
{
    if (_pMgHttpMessage == NULL)
        return _strNullData;

    return _pMgHttpMessage->uri.p;
}

string ccMongooseWebServerRequest::GetQueryString() const
{
    if (_pMgHttpMessage == NULL)
        return _strNullData;

    return _pMgHttpMessage->query_string.p;
}

string ccMongooseWebServerRequest::GetPath() const
{
    if (_pMgHttpMessage == NULL)
        return _strNullData;

    DoSplitePathPos();

    return string(_pMgHttpMessage->uri.p, _pPathPos);
}

string ccMongooseWebServerRequest::GetResource() const
{
    if (_pMgConnection == NULL)
        return _strNullData;

    DoSplitePathPos();

    return string(_pPathPos, _pathEnd);
}

bool ccMongooseWebServerRequest::HasVar(const string& name) const
{
    return DoHasVarInConnection(name);
}


string ccMongooseWebServerRequest::GetVar(const string& name) const
{
    return DoGetVarInConnection(name);
}

string ccMongooseWebServerRequest::GetHeader(const string& name) const
{
    if (_pMgHttpMessage == NULL)
        return _strNullData;

    struct mg_str* pResult = mg_get_http_header(_pMgHttpMessage, name.c_str());

    if (pResult == NULL)
        return _strNullData;

    return pResult->p;
}

string ccMongooseWebServerRequest::GetContentType() const
{
    return GetHeader("Content-Type");
}

unsigned long ccMongooseWebServerRequest::GetContentLength() const
{
    return atol(GetHeader("Content-Length").c_str());
}

// Retrieve info for the server interface that was connected with the remote device.
long ccMongooseWebServerRequest::GetServerIp() const
{
    return 0;
}

unsigned short  ccMongooseWebServerRequest::GetServerPort() const
{
    return 0;
}

long ccMongooseWebServerRequest::GetRemoteIp() const
{
    return 0;
}

const char* ccMongooseWebServerRequest::GetPostData(unsigned long* size) const
{
    if (size != 0)  // 0 means NULL
       *size = 0;

    return _strNullData.c_str();
}

//  private method
void ccMongooseWebServerRequest::DoSplitePathPos() const
{
    if (_pPathPos || _pMgHttpMessage == NULL)
        return;

    char const * slash = 0;
    char const * it = _pMgHttpMessage->uri.p;

    while (*it)
    {
        if (*it == '/')
            slash = it;

        ++it;
    }

    _pathEnd    = it;
    _pPathPos   = slash == 0 ? _pathEnd : slash + 1;
}

bool ccMongooseWebServerRequest::DoHasVarInConnection(const string& name) const
{
    if (_pMgHttpMessage == NULL)
        return false;

    char outbuf[CV_MAXGETSIZE];

    return mg_get_http_var(&_pMgHttpMessage->body, name.c_str(), &outbuf[0], CV_MAXGETSIZE) > 0;
}

string ccMongooseWebServerRequest::DoGetVarInConnection(const string& name) const
{
    string strGetData;

    strGetData.resize(CV_MAXGETSIZE);

    int sz = -1;
    
    if (_pMgHttpMessage != NULL)
        mg_get_http_var(&_pMgHttpMessage->body, name.c_str(), &strGetData[0], strGetData.size());

    if (sz == -1)
        return _strNullData;

    strGetData.resize(sz);

    return strGetData;
}
