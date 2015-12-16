/*
 * ccMongooseWebServerRequest.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#ifndef THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERREQUEST_H_
#define THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERREQUEST_H_

#include "CCWebServerAPI/ccWebServerRequest.h"

#include "mongoose/mongoose.h"

#include <vector>

class ccMongooseWebServerRequest : public ccWebServerRequest
{
public:
    ccMongooseWebServerRequest(struct mg_connection* con, struct http_message* http_msg);
    virtual ~ccMongooseWebServerRequest();

public:
    virtual const string&   GetQueryString() const;
    virtual string          GetPath() const;
    virtual string          GetResource() const;

    virtual bool        HasVar(const string& name) const;
    virtual string      GetVar(const string& name) const;

    virtual string      GetHeader(const string& name) const;

    virtual string          GetContentType() const;
    virtual unsigned long   GetContentLength() const;

    virtual long            GetContentBody(std::string& strBody);
    virtual long            GetContentBody(char* pBuf, std::size_t size);

    // Retrieve info for the server interface that was connected with the remote device.
    virtual long            GetServerIp() const;
    virtual unsigned short  GetServerPort() const;
    virtual long            GetRemoteIp() const;

private:
    void        DoSplitePathPos() const;

    bool        DoHasVarInConnection(const string& name) const;
    string      DoGetVarInConnection(const string& name) const;

private:
    struct mg_connection*           _pMgConnection;
    struct http_message*            _pMgHttpMessage;

    mutable char const*             _pPathPos;
    mutable char const*             _pathEnd;

    mutable std::vector<char>       _pPostBuffer;
};

#endif /* THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERREQUEST_H_ */
