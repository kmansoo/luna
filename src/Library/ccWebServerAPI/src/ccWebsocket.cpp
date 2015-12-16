/*
* ccWebsocket.cpp
*
*  Created on: 2015. 12. 14.
*      Author: kmansoo
*/

#include "ccWebsocketGroup.h"

#include "ccWebsocket.h"

ccWebsocket::ccWebsocket(const std::string& strUri)
    : _pMyGroup(NULL)
{
    _strUri = strUri;
}

ccWebsocket::ccWebsocket(const char* pUri, std::size_t size)
    : _pMyGroup(NULL)
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

void ccWebsocket::SetGroup(ccWebsocketGroup* pGroup)
{
    _pMyGroup = pGroup;
}

ccWebsocketGroup* ccWebsocket::GetGroup()
{
    return _pMyGroup;
}

bool ccWebsocket::Send(const std::string& strMessage)
{
    return Send(strMessage.c_str(), strMessage.length());
}
