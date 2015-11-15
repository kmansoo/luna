/*
 * ccWebServerManager.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include <algorithm>

#include "../ccWebServerManager.h"

ccWebServerManager::ccWebServerManager()
{
    // TODO Auto-generated constructor stub

}

ccWebServerManager::~ccWebServerManager()
{
    // TODO Auto-generated destructor stub
}

void  ccWebServerManager::AttachFactory(std::shared_ptr<ccWebServerObjectFactory> pFactory)
{
    _pObjFactory = pFactory;
}

bool ccWebServerManager::CreateWebServer(const char* name, const char* ports)
{
    if (_pObjFactory == nullptr)
        return false;

    _aWebServers.push_back(_pObjFactory->CreateWebServer(name, ports));

    return true;
}

shared_ptr<ccWebServer> ccWebServerManager::GetWebServer(const char* strName)
{
    for (const auto& server : _aWebServers)
    {
        if (server->GetName().compare(strName) == 0)
            return server;
    }

    return nullptr;
}

void ccWebServerManager::Start()
{
    for (const auto& server : _aWebServers)
    {
        server->SetListener(this);
        server->Start();
    }
}

void ccWebServerManager::Stop()
{
    for (const auto& server : _aWebServers)
    {
        server->SetListener(NULL);
        server->Stop();
    }
}

bool ccWebServerManager::OnWebServerRequest(ccWebServerRequest* request, ccWebServerResponse* response)
{

    return false;
}
