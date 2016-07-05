
#ifndef THIRDPARTYLIBRARY_CCBOOSTSERVERAPI_CCBOOSTWEBSERVERREQUEST_H_
#define THIRDPARTYLIBRARY_CCBOOSTSERVERAPI_CCBOOSTWEBSERVERREQUEST_H_

#include "CCWebServerAPI/ccWebServerRequest.h"
#include <vector>

class ccBoostWebServerRequest : public ccWebServerRequest
{
public:
    ccBoostWebServerRequest(const string& requestHeader, const string& requestBody);
    virtual ~ccBoostWebServerRequest();

public:
    virtual HttpMethod  GetMethod() const;
    virtual string      GetMethod_c() const;

    virtual string      GetURI() const;
    virtual string      GetQueryString() const;
    virtual string      GetPath() const;
    virtual string      GetResource() const;

    virtual bool        HasVar(const string& name) const;
    virtual string      GetVar(const string& name) const;

    virtual string      GetHeader(const string& name) const;

    virtual string          GetContentType() const;
    virtual unsigned long   GetContentLength() const;

    // Retrieve info for the server interface that was connected with the remote device.
    virtual long            GetServerIp() const;
    virtual unsigned short  GetServerPort() const;
    virtual long            GetRemoteIp() const;

    virtual const char* GetPostData(unsigned long* size = NULL) const;

private:
	const string _requestHeader;
	const string _requestBody;
};

#endif /* THIRDPARTYLIBRARY_CCBOOSTSERVERAPI_CCBOOSTWEBSERVERREQUEST_H_ */
