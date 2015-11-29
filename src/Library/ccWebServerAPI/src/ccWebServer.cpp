/*
 * ccWebServer.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include "../ccWebServer.h"

ccWebServer::ccWebServer(const char* name, const char* ports) :
    _strName(name),
    _strHttpPorts(ports),
    _pEventListener(NULL)
{
    // TODO Auto-generated constructor stub

}

ccWebServer::~ccWebServer()
{
    // TODO Auto-generated destructor stub
}

bool ccWebServer::Start()
{
    return false;
}

bool ccWebServer::Stop()
{
    return false;
}

const string&  ccWebServer::GetName()
{
    return _strName;
}

bool ccWebServer::SetOption(string const& name, string const& value)
{
    _aOptions.push_back(name);
    _aOptions.push_back(value);

    return true;
}

void ccWebServer::SetListener(ccWebServerEventListener* pListener)
{
    _pEventListener = pListener;
}

