/*
 * ccWebServerRequest.h
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERREQUEST_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERREQUEST_H_

#include <string>

using namespace std;

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
    virtual HttpMethod      GetMethod() const;
    virtual const string&   GetMethod_c() const;

    virtual const string&   GetURI() const;
    virtual const string&   GetQueryString() const;
    virtual string          GetPath() const;
    virtual string          GetResource() const;

    virtual bool        HasVar(const string& name) const;
    virtual string      GetVar(const string& name) const;

    virtual string      GetHeader(const string& name) const;

    virtual string          GetContentType() const;
    virtual unsigned long   GetContentLength() const;

    // Retrieve info for the server interface that was connected with the remote device.
    virtual long            GetServerIp() const;
    virtual unsigned short  GetServerPort() const;
    virtual long            GetRemoteIp() const;

    virtual const char*     GetPostData(unsigned long* size = NULL) const;

protected:
    string  _strNullData;

    std::string _strMethod;
    std::string _strUri;
};

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERREQUEST_H_ */
