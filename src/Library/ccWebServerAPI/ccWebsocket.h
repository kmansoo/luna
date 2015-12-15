/*
* ccWebsocket.h
*
*  Created on: 2015. 12. 14.
*      Author: kmansoo
*/

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSOCKET_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSOCKET_H_

#include <string>

class ccWebsocket
{
public:
    ccWebsocket(const std::string& strUri);
    ccWebsocket(const char* pUri, std::size_t size);
    virtual ~ccWebsocket();

public:
    const std::string   GetUri();

public:
    virtual std::int32_t    GetInstance() = 0;  // It may be a Socket ID. 
    virtual bool            Send(const char* strMessage, std::size_t size) = 0;
    virtual bool            Send(const std::string& strMessage);

protected:
    std::string _strUri;
};

#endif
