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


ccWebsocket::~ccWebsocket()
{
}

const std::string ccWebsocket::GetUri()
{
    return _strUri;
}
