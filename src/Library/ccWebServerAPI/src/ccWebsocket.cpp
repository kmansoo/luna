/*
* ccWebsocket.cpp
*
*  Created on: 2015. 12. 14.
*      Author: kmansoo
*/

#include "ccWebsocket.h"

ccWebsocket::ccWebsocket(const std::string& strUri)
{
    _strUri = strUri;
}

ccWebsocket::ccWebsocket(const char* pUri, std::size_t size)
{
    _strUri.assign(pUri, size);
}

ccWebsocket::~ccWebsocket()
{
}

const std::string ccWebsocket::GetUri()
{
    return _strUri;
}

bool ccWebsocket::Send(const std::string& strMessage)
{
    return Send(strMessage.c_str(), strMessage.length());
}
