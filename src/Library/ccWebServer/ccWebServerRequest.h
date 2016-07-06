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
    virtual HttpMethod          getMethod() const;
    virtual const std::string&  getMethod_c() const;
    virtual const std::string&  getURI() const;

    virtual std::string         getQueryString();
    virtual std::string         getPath();
    virtual std::string         getResource();

    virtual bool                hasVar(const std::string& name) const;
    virtual std::string         getVar(const std::string& name);

    virtual std::string         getHeader(const std::string& name);

    virtual std::string         getContentType();
    virtual unsigned long       getContentLength();

    virtual long                getContentBody(std::string& strBody);
    virtual long                getContentBody(char* pBuf, std::size_t size);

    // Retrieve info for the server interface that was connected with the remote device.
    virtual long                getServerIp() const;
    virtual unsigned short      getServerPort() const;
    virtual long                getRemoteIp() const;

protected:
    std::string _strNullData;

    std::string _strMethod;
    std::string _strUri;
};

}

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERREQUEST_H_ */
