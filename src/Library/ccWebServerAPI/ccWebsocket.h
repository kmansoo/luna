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
    virtual ~ccWebsocket();

public:
    const std::string   GetUri();

public:
    virtual bool    Send(const char* strMessage, std::size_t size) = 0;
    virtual bool    Send(const std::string& strMessage) = 0;

protected:
    std::string _strUri;
};

#endif
