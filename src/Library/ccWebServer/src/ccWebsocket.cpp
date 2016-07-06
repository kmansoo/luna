/*
* ccWebsocket.cpp
*
*  Created on: 2015. 12. 14.
*      Author: kmansoo
*/

#include "ccWebsocketGroup.h"

#include "ccWebsocket.h"

namespace Luna {

ccWebsocket::ccWebsocket(const std::string& strUri)
    : _pMyGroup(NULL)
{
    _strUri = strUri;
}

ccWebsocket::~ccWebsocket()
{
}

void  ccWebsocket::SetEventListener(WebSocketEventFunction f)
{
    _oEventListener = f;
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
    return false;
}

}
