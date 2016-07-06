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

void  ccWebsocket::setEventListener(WebSocketEventFunction f)
{
    _oEventListener = f;
}


const std::string ccWebsocket::getUri()
{
    return _strUri;
}

void ccWebsocket::setGroup(ccWebsocketGroup* pGroup)
{
    _pMyGroup = pGroup;
}

ccWebsocketGroup* ccWebsocket::getGroup()
{
    return _pMyGroup;
}

bool ccWebsocket::send(const std::string& strMessage)
{
    return false;
}

}
