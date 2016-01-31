/*
 * ccMongooseWebServerRequest.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#ifndef THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERREQUEST_H_
#define THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERREQUEST_H_

#include "ccWebServerAPI/ccWebServerRequest.h"

#include "mongoose/mongoose.h"

#include <vector>

class ccMongooseWebServerRequest : public ccWebServerRequest
{
public:
    ccMongooseWebServerRequest(struct mg_connection* con, struct http_message* http_msg);
    virtual ~ccMongooseWebServerRequest();

public:
    virtual std::string     GetQueryString();
    virtual std::string     GetPath();
    virtual std::string     GetResource();

    virtual bool            HasVar(const std::string& name) const;
    virtual std::string     GetVar(const std::string& name);

    virtual std::string     GetHeader(const std::string& name);

    virtual std::string     GetContentType();
    virtual unsigned long   GetContentLength();

    virtual long            GetContentBody(std::string& strBody);
    virtual long            GetContentBody(char* pBuf, std::size_t size);

    // Retrieve info for the server interface that was connected with the remote device.
    virtual long            GetServerIp() const;
    virtual unsigned short  GetServerPort() const;
    virtual long            GetRemoteIp() const;

private:
    void            DoSplitePathPos() const;

    bool            DoHasVarInConnection(const std::string& name) const;
    std::string     DoGetVarInConnection(const std::string& name);

private:
    struct mg_connection*           _pMgConnection;
    struct http_message*            _pMgHttpMessage;

    mutable char const*             _pPathPos;
    mutable char const*             _pathEnd;

    mutable std::vector<char>       _pPostBuffer;
};

#endif /* THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERREQUEST_H_ */
