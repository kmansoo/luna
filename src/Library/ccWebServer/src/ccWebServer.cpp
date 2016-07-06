/*
 * ccWebServer.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include "../ccWebServer.h"

namespace Luna {

ccWebServer::ccWebServer(const std::string& name, const std::string& ports, const std::string& root_path) :
    _strName(name),
    _strHttpPorts(ports),
    _strWebRootPath(root_path),
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

const std::string&  ccWebServer::GetName()
{
    return _strName;
}

bool ccWebServer::SetOption(std::string const& name, std::string const& value)
{
    _aOptions.push_back(name);
    _aOptions.push_back(value);

    return true;
}

void ccWebServer::SetListener(ccWebServerEventListener* pListener)
{
    _pEventListener = pListener;
}

}
