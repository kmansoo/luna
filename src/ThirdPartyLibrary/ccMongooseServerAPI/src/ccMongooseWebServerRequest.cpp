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

const std::string&& ccMongooseWebServerRequest::GetQueryString()
{
    if (_pMgHttpMessage == NULL)
        return std::move(_strNullData);

    std::string strQuery(_pMgHttpMessage->query_string.p, _pMgHttpMessage->query_string.len);

    return std::move(strQuery);
}

std::string&& ccMongooseWebServerRequest::GetPath()
{
    if (_pMgHttpMessage == NULL)
        return std::move(_strNullData);

    DoSplitePathPos();

    std::string strPath = _strUri;

    if (_pPathPos != NULL)
        strPath += _pPathPos;

    return std::move(strPath);
}

std::string&& ccMongooseWebServerRequest::GetResource()
{
    if (_pMgConnection == NULL)
        return std::move(_strNullData);

    DoSplitePathPos();

    return std::move(std::string(_pPathPos, _pathEnd));
}

bool ccMongooseWebServerRequest::HasVar(const std::string& name) const
{
    return DoHasVarInConnection(name);
}


std::string&& ccMongooseWebServerRequest::GetVar(const std::string& name)
{
    return DoGetVarInConnection(name);
}

std::string&& ccMongooseWebServerRequest::GetHeader(const std::string& name)
{
    if (_pMgHttpMessage == NULL)
        return std::move(_strNullData);

    struct mg_str* pResult = mg_get_http_header(_pMgHttpMessage, name.c_str());

    if (pResult == NULL)
        return std::move(_strNullData);

    std::string strHeader(pResult->p, pResult->len);

    return std::move(strHeader);
}

std::string&& ccMongooseWebServerRequest::GetContentType()
{
    return GetHeader("Content-Type");
}

unsigned long ccMongooseWebServerRequest::GetContentLength()
{
    std::string strContentLength = GetHeader("Content-Length");

    return atol(strContentLength.c_str());
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

bool ccMongooseWebServerRequest::DoHasVarInConnection(const std::string& name) const
{
    if (_pMgHttpMessage == NULL)
        return false;

    char outbuf[CV_MAXGETSIZE];

    return mg_get_http_var(&_pMgHttpMessage->body, name.c_str(), &outbuf[0], CV_MAXGETSIZE) > 0;
}

std::string&& ccMongooseWebServerRequest::DoGetVarInConnection(const std::string& name)
{
    std::string strGetData;

    strGetData.resize(CV_MAXGETSIZE);

    int sz = -1;
    
    if (_pMgHttpMessage != NULL)
        sz = mg_get_http_var(&_pMgHttpMessage->query_string, name.c_str(), &strGetData[0], strGetData.size());

    if (sz == -1)
        return std::move(_strNullData);

    strGetData.resize(sz);

    return std::move(strGetData);
}
