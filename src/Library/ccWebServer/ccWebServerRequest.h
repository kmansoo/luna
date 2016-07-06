/*
 * ccWebServerRequest.h
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERREQUEST_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERREQUEST_H_

#include <string>

namespace Luna {

class ccWebServerRequest
{
public:
    ccWebServerRequest();
    virtual ~ccWebServerRequest();

public:
    enum HttpMethod {
        HttpMethod_Unknown,
        HttpMethod_Get,
        HttpMethod_Head,
        HttpMethod_Post,
        HttpMethod_Put,
        HttpMethod_Delete,
        HttpMethod_Trace,
        HttpMethod_Connect
    };

    enum CONST_VALUE {
        CV_MAXGETSIZE = 1024
    };


public:
    virtual HttpMethod          GetMethod() const;
    virtual const std::string&  GetMethod_c() const;
    virtual const std::string&  GetURI() const;

    virtual std::string         GetQueryString();
    virtual std::string         GetPath();
    virtual std::string         GetResource();

    virtual bool                HasVar(const std::string& name) const;
    virtual std::string         GetVar(const std::string& name);

    virtual std::string         GetHeader(const std::string& name);

    virtual std::string         GetContentType();
    virtual unsigned long       GetContentLength();

    virtual long                GetContentBody(std::string& strBody);
    virtual long                GetContentBody(char* pBuf, std::size_t size);

    // Retrieve info for the server interface that was connected with the remote device.
    virtual long                GetServerIp() const;
    virtual unsigned short      GetServerPort() const;
    virtual long                GetRemoteIp() const;

protected:
    std::string _strNullData;

    std::string _strMethod;
    std::string _strUri;
};

}

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERREQUEST_H_ */
