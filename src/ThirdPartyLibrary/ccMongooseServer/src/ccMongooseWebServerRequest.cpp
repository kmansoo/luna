/*
 * ccMongooseWebServerRequest.cpp
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#include "ccMongooseWebServerRequest.h"

namespace Luna {

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

std::string ccMongooseWebServerRequest::getQueryString()
{
    if (_pMgHttpMessage == NULL)
        return _strNullData;

    std::string strQuery(_pMgHttpMessage->query_string.p, _pMgHttpMessage->query_string.len);

    return strQuery;
}

std::string ccMongooseWebServerRequest::getPath()
{
    if (_pMgHttpMessage == NULL)
        return _strNullData;

    doSplitePathPos();

    std::string strPath = _strUri;

    if (_pPathPos != NULL)
        strPath += _pPathPos;

    return strPath;
}

std::string ccMongooseWebServerRequest::getResource()
{
    if (_pMgConnection == NULL)
        return _strNullData;

    doSplitePathPos();

    return std::string(_pPathPos, _pathEnd);
}

bool ccMongooseWebServerRequest::hasVar(const std::string& name) const
{
    return doHasVarInConnection(name);
}


std::string ccMongooseWebServerRequest::getVar(const std::string& name)
{
    return doGetVarInConnection(name);
}

std::string ccMongooseWebServerRequest::getHeader(const std::string& name)
{
    if (_pMgHttpMessage == NULL)
        return _strNullData;

    struct mg_str* pResult = mg_get_http_header(_pMgHttpMessage, name.c_str());

    if (pResult == NULL)
        return _strNullData;

    std::string strHeader(pResult->p, pResult->len);

    return strHeader;
}

std::string ccMongooseWebServerRequest::getContentType()
{
    return getHeader("Content-Type");
}

unsigned long ccMongooseWebServerRequest::getContentLength()
{
    std::string strContentLength = getHeader("Content-Length");

    return atol(strContentLength.c_str());
}

// Retrieve info for the server interface that was connected with the remote device.
long ccMongooseWebServerRequest::getServerIp() const
{
    return 0;
}

unsigned short  ccMongooseWebServerRequest::getServerPort() const
{
    return 0;
}

long ccMongooseWebServerRequest::getRemoteIp() const
{
    return 0;
}

long ccMongooseWebServerRequest::getContentBody(std::string& strBody)
{
    if (_pMgHttpMessage == NULL)
        return 0;

    strBody.append(_pMgHttpMessage->body.p, _pMgHttpMessage->body.len);

    return _pMgHttpMessage->body.len;
}

long ccMongooseWebServerRequest::getContentBody(char* pBuf, std::size_t size)
{
    if (_pMgHttpMessage == NULL || pBuf == NULL)
        return 0;

    std::size_t copy_size = (size > _pMgHttpMessage->body.len) ? _pMgHttpMessage->body.len : size;

    memcpy(pBuf, _pMgHttpMessage->body.p, copy_size);

    return copy_size;
}

//  private method
void ccMongooseWebServerRequest::doSplitePathPos() const
{
    if (_pPathPos || _pMgHttpMessage == NULL)
        return;

    char const * slash = 0;
    char const * it = _pMgHttpMessage->uri.p;

    for (std::size_t nIndex = 0; nIndex < _pMgHttpMessage->uri.len; nIndex++)
    {
        if (*it == '/')
            slash = it;

        ++it;
    }

    _pathEnd    = it;
    _pPathPos   = slash == 0 ? _pathEnd : slash + 1;
}

bool ccMongooseWebServerRequest::doHasVarInConnection(const std::string& name) const
{
    if (_pMgHttpMessage == NULL)
        return false;

    char outbuf[CV_MAXGETSIZE];

    return mg_get_http_var(&_pMgHttpMessage->body, name.c_str(), &outbuf[0], CV_MAXGETSIZE) > 0;
}

std::string ccMongooseWebServerRequest::doGetVarInConnection(const std::string& name)
{
    std::string strGetData;

    strGetData.resize(CV_MAXGETSIZE);

    int sz = -1;
    
    if (_pMgHttpMessage != NULL)
        sz = mg_get_http_var(&_pMgHttpMessage->query_string, name.c_str(), &strGetData[0], strGetData.size());

    if (sz == -1)
        return _strNullData;

    strGetData.resize(sz);

    return strGetData;
}

}
