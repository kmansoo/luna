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

bool ccWebServerManager::AddRESTfulApi(shared_ptr<ccRESTfulApi> pNewAPI)
{
    for (const auto& api : _aWebAPIs)
    {
        if (api == pNewAPI)
            return false;
    }

    _aWebAPIs.push_back(pNewAPI);

    return true;
}

bool ccWebServerManager::RemoveRESTfulApi(shared_ptr<ccRESTfulApi> pNewAPI)
{
    //for (const auto& api : _aWebAPIs)
    //{
    //    if (api == pNewAPI)
    //    {
    //        _aWebAPIs.erase(api);

    //        return true;
    //    }
    //}

    return false;
}

bool ccWebServerManager::RemoveAllRESTfulApi()
{
    _aWebAPIs.clear();

    return true;
}

bool ccWebServerManager::OnWebServerRequest(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response)
{
    for (const auto& api : _aWebAPIs)
    {
        if (api->HasAPI(request->GetURI()) == true)
            return api->ExecuteAPI(request, response);
    }

    return false;
}
