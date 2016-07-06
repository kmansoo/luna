/*
 * ccMongooseWebServerRequest.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#ifndef THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERREQUEST_H_
#define THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERREQUEST_H_

#include "ccWebServer/ccWebServerRequest.h"

#include "mongoose/mongoose.h"

#include <vector>

namespace Luna {

class ccMongooseWebServerRequest : public ccWebServerRequest
{
public:
    ccMongooseWebServerRequest(struct mg_connection* con, struct http_message* http_msg);
    virtual ~ccMongooseWebServerRequest();

public:
    virtual std::string     getQueryString();
    virtual std::string     getPath();
    virtual std::string     getResource();

    virtual bool            hasVar(const std::string& name) const;
    virtual std::string     getVar(const std::string& name);

    virtual std::string     getHeader(const std::string& name);

    virtual std::string     getContentType();
    virtual unsigned long   getContentLength();

    virtual long            getContentBody(std::string& strBody);
    virtual long            getContentBody(char* pBuf, std::size_t size);

    // Retrieve info for the server interface that was connected with the remote device.
    virtual long            getServerIp() const;
    virtual unsigned short  getServerPort() const;
    virtual long            getRemoteIp() const;

private:
    void            doSplitePathPos() const;

    bool            doHasVarInConnection(const std::string& name) const;
    std::string     doGetVarInConnection(const std::string& name);

private:
    struct mg_connection*           _pMgConnection;
    struct http_message*            _pMgHttpMessage;

    mutable char const*             _pPathPos;
    mutable char const*             _pathEnd;

    mutable std::vector<char>       _pPostBuffer;
};

}

#endif /* THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERREQUEST_H_ */
