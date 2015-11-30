/*
 * ccMongooseWebServerRequest.cpp
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#include "ccMongooseWebServerRequest.h"

ccMongooseWebServerRequest::ccMongooseWebServerRequest(struct mg_connection* con, struct http_message* http_msg)
    : _pMgConnection(con),
      _pMgHttpMessage(http_msg),
      _pPathPos(NULL),
      _pathEnd(NULL)
{
    if (_pMgHttpMessage != NULL)
    {
        _strMethod.assign(_pMgHttpMessage->method.p, _pMgHttpMessage->method.len);
        _strUri.assign(_pMgHttpMessage->uri.p, _pMgHttpMessage->uri.len);
    }
}

ccMongooseWebServerRequest::~ccMongooseWebServerRequest()
{
    // TODO Auto-generated destructor stub
}

//ccMongooseWebServerRequest::HttpMethod  ccMongooseWebServerRequest::GetMethod() const
//{
//    if (_pMgHttpMessage == NULL)
//        return HttpMethod_Unknown;
//
//    //if (mg_vcmp(&_pMgHttpMessage->method, "GET") == 0)return HttpMethod_Get;
//    //if (mg_vcmp(&_pMgHttpMessage->method, "HEAD") == 0)return HttpMethod_Head;
//    //if (mg_vcmp(&_pMgHttpMessage->method, "POST") == 0)return HttpMethod_Post;
//    //if (mg_vcmp(&_pMgHttpMessage->method, "PUT") == 0)return HttpMethod_Put;
//    //if (mg_vcmp(&_pMgHttpMessage->method, "DELETE") == 0)return HttpMethod_Delete;
//    //if (mg_vcmp(&_pMgHttpMessage->method, "TRACE") == 0)return HttpMethod_Trace;
//    //if (mg_vcmp(&_pMgHttpMessage->method, "CONNECT") == 0)return HttpMethod_Connect;
//
//    if (_strMethod == "GET")        return HttpMethod_Get;
//    if (_strMethod == "HEAD")       return HttpMethod_Head;
//    if (_strMethod == "POST")       return HttpMethod_Post;
//    if (_strMethod == "PUT")        return HttpMethod_Put;
//    if (_strMethod == "DELETE")     return HttpMethod_Delete;
//    if (_strMethod == "TRACE")      return HttpMethod_Trace;
//    if (_strMethod == "CONNECT")    return HttpMethod_Connect;
//
//    return HttpMethod_Unknown;
//}

const string& ccMongooseWebServerRequest::GetQueryString() const
{
    if (_pMgHttpMessage == NULL)
        return _strNullData;

    std::string strQuery(_pMgHttpMessage->query_string.p, _pMgHttpMessage->query_string.len);

    return strQuery;
}

string ccMongooseWebServerRequest::GetPath() const
{
    if (_pMgHttpMessage == NULL)
        return _strNullData;

    DoSplitePathPos();

    std::string strPath = _strUri;

    if (_pPathPos != NULL)
        strPath += _pPathPos;

    return strPath;
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

    std::string strHeader(pResult->p, pResult->len);

    return strHeader;
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

long ccMongooseWebServerRequest::GetContentBody(std::string& strBody)
{
    if (_pMgHttpMessage == NULL)
        return 0;

    strBody.append(_pMgHttpMessage->body.p, _pMgHttpMessage->body.len);

    return _pMgHttpMessage->body.len;
}

long ccMongooseWebServerRequest::GetContentBody(char* pBuf, std::size_t size)
{
    if (_pMgHttpMessage == NULL || pBuf == NULL)
        return 0;

    std::size_t copy_size = (size > _pMgHttpMessage->body.len) ? _pMgHttpMessage->body.len : size;

    memcpy(pBuf, _pMgHttpMessage->body.p, copy_size);

    return copy_size;
}

//  private method
void ccMongooseWebServerRequest::DoSplitePathPos() const
{
    //if (_pPathPos || _pMgHttpMessage == NULL)
    //    return;

    //char const * slash = 0;
    //char const * it = _pMgHttpMessage->uri.p;

    //for (int nIndex = 0; nIndex < _pMgHttpMessage->uri.len; nIndex++)
    //{
    //    if (*it == '/')
    //        slash = it;

    //    ++it;
    //}

    //_pathEnd    = it;
    //_pPathPos   = slash == 0 ? _pathEnd : slash + 1;
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
