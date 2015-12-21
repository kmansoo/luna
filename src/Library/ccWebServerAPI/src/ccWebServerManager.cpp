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

bool ccWebServerManager::CreateWebServer(const std::string& name, const std::string& ports)
{
    if (_pObjFactory == nullptr)
        return false;

    _aWebServers.push_back(_pObjFactory->CreateWebServer(name, ports, "."));

    return true;
}

bool ccWebServerManager::CreateWebServer(const std::string& name, const std::string& ports, const std::string& root_path)
{
    if (_pObjFactory == nullptr)
        return false;

    _aWebServers.push_back(_pObjFactory->CreateWebServer(name, ports, root_path));

    return true;
}


shared_ptr<ccWebServer> ccWebServerManager::GetWebServer(const char* strName)
{
    for (auto server : _aWebServers)
    {
        if (server->GetName().compare(strName) == 0)
            return server;
    }

    return nullptr;
}

void ccWebServerManager::Start()
{
    for (auto server : _aWebServers)
    {
        server->SetListener(this);
        server->Start();
    }
}

void ccWebServerManager::Stop()
{
    for (const auto server : _aWebServers)
    {
        server->SetListener(NULL);
        server->Stop();
    }
}

bool ccWebServerManager::AddRESTfulApi(shared_ptr<ccRESTfulApi> pNewAPI)
{
    for (auto item : _aWebAPIs)
    {
        if (item == pNewAPI)
            return false;
    }

    _aWebAPIs.push_back(pNewAPI);

    return true;
}

bool ccWebServerManager::RemoveRESTfulApi(shared_ptr<ccRESTfulApi> pNewAPI)
{
    auto it = std::find(std::begin(_aWebAPIs), std::end(_aWebAPIs), pNewAPI);

    if (it != std::end(_aWebAPIs))
    {
        _aWebAPIs.erase(it);

        return true;
    }

    return false;
}

bool ccWebServerManager::RemoveAllRESTfulApi()
{
    _aWebAPIs.clear();

    return true;
}

bool ccWebServerManager::AddWebsocketManager(shared_ptr<ccWebsocketManager> pNewWSGM)
{
    for (const auto& item : _aWSMs)
    {
        if (item == pNewWSGM)
            return false;
    }

    _aWSMs.push_back(pNewWSGM);

    return true;
}


bool ccWebServerManager::RemoveWebsocketManager(shared_ptr<ccWebsocketManager> pNewWSGM)
{
    auto it = std::find(std::begin(_aWSMs), std::end(_aWSMs), pNewWSGM);

    if (it != std::end(_aWSMs))
    {
        _aWSMs.erase(it);

        return true;
    }

    return false;
}

bool ccWebServerManager::RemoveAllWebsocketManager()
{
    _aWSMs.clear();

    return true;
}

bool ccWebServerManager::OnWebServerRequest(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response)
{
    for (const auto& api : _aWebAPIs)
    {
        if (api->HasAPI(request->GetURI()) == true)
            return api->PerformAPI(request, response);
    }

    return false;
}

bool ccWebServerManager::OnNewWebsocketRequest(const std::string& strWebsocketUri)
{
    if (_aWSMs.size() == 0)
        return false;

    for (const auto& item : _aWSMs)
    {
        if (item->HasUri(strWebsocketUri))
            return true;
    }

    return false;
}

void ccWebServerManager::OnWebsocketCreated(std::shared_ptr<ccWebsocket> newWebsocket)
{
    for (const auto& item : _aWSMs)
    {
        if (item->HasUri(newWebsocket->GetUri()))
        {
            item->AddWebsocket(newWebsocket);
            return;
        }
    }
}

void ccWebServerManager::OnWebsocketConnected(std::int32_t socketID)
{
    DoPerformWebsocketEvent(ccWebsocket::ccWebSocketEvent_Connected, socketID);
}

void ccWebServerManager::OnWebsocketClosed(std::int32_t socketID)
{
    DoPerformWebsocketEvent(ccWebsocket::ccWebSocketEvent_Close, socketID);
}

void ccWebServerManager::OnWebsocketData(std::int32_t socketID, const char* pData, std::size_t size)
{
    DoPerformWebsocketEvent(ccWebsocket::ccWebSocketEvent_Data, socketID, pData, size);
}

void ccWebServerManager::DoPerformWebsocketEvent(ccWebsocket::ccWebSocketEvent eEvent, std::int32_t socketID, const char* pData, std::size_t size)
{
    std::shared_ptr<ccWebsocket> newWebsocket;

    for (const auto& item : _aWSMs)
    {
        if (item->GetWebsocket(socketID, newWebsocket))
        {
            if (newWebsocket != NULL)
                item->PerformWebsocketEvent(eEvent, newWebsocket, pData, size);

            return;
        }
    }
}
