/*
 * ccWebServerManager.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include <algorithm>

#include "../ccWebServerManager.h"

namespace Luna {

ccWebServerManager::ccWebServerManager()
{
    // TODO Auto-generated constructor stub

}

ccWebServerManager::~ccWebServerManager()
{
    // TODO Auto-generated destructor stub
}

void  ccWebServerManager::attachFactory(std::shared_ptr<ccWebServerObjectFactory> pFactory)
{
    _pObjFactory = pFactory;
}

bool ccWebServerManager::createWebServer(const std::string& name, const std::string& ports)
{
    if (_pObjFactory == nullptr)
        return false;

    _aWebServers.push_back(_pObjFactory->createWebServer(name, ports, "."));

    return true;
}

bool ccWebServerManager::createWebServer(const std::string& name, const std::string& ports, const std::string& root_path)
{
    if (_pObjFactory == nullptr)
        return false;

    _aWebServers.push_back(_pObjFactory->createWebServer(name, ports, root_path));

    return true;
}


std::shared_ptr<ccWebServer> ccWebServerManager::getWebServer(const char* strName)
{
    for (auto server : _aWebServers)
    {
        if (server->getName().compare(strName) == 0)
            return server;
    }

    return nullptr;
}

void ccWebServerManager::start()
{
    for (auto server : _aWebServers)
    {
        server->setListener(this);
        server->start();
    }
}

void ccWebServerManager::stop()
{
    for (const auto server : _aWebServers)
    {
        server->setListener(NULL);
        server->stop();
    }
}

bool ccWebServerManager::addRESTfulApi(std::shared_ptr<ccRESTfulApi> pNewAPI)
{
    for (auto item : _aWebAPIs)
    {
        if (item == pNewAPI)
            return false;
    }

    _aWebAPIs.push_back(pNewAPI);

    return true;
}

bool ccWebServerManager::removeRESTfulApi(std::shared_ptr<ccRESTfulApi> pNewAPI)
{
    auto it = std::find(std::begin(_aWebAPIs), std::end(_aWebAPIs), pNewAPI);

    if (it != std::end(_aWebAPIs))
    {
        _aWebAPIs.erase(it);

        return true;
    }

    return false;
}

bool ccWebServerManager::removeAllRESTfulApi()
{
    _aWebAPIs.clear();

    return true;
}

bool ccWebServerManager::addWebsocketManager(std::shared_ptr<ccWebsocketManager> pNewWSGM)
{
    for (const auto& item : _aWSMs)
    {
        if (item == pNewWSGM)
            return false;
    }

    _aWSMs.push_back(pNewWSGM);

    return true;
}


bool ccWebServerManager::removeWebsocketManager(std::shared_ptr<ccWebsocketManager> pNewWSGM)
{
    auto it = std::find(std::begin(_aWSMs), std::end(_aWSMs), pNewWSGM);

    if (it != std::end(_aWSMs))
    {
        _aWSMs.erase(it);

        return true;
    }

    return false;
}

bool ccWebServerManager::removeAllWebsocketManager()
{
    _aWSMs.clear();

    return true;
}

bool ccWebServerManager::onWebServerRequest(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response)
{
    for (const auto& api : _aWebAPIs)
    {
        if (api->hasAPI(request->getURI()) == true)
            return api->performAPI(request, response);
    }

    return false;
}

bool ccWebServerManager::onNewWebsocketRequest(const std::string& strWebsocketUri)
{
    if (_aWSMs.size() == 0)
        return false;

    for (const auto& item : _aWSMs)
    {
        if (item->hasUri(strWebsocketUri))
            return true;
    }

    return false;
}

void ccWebServerManager::onWebsocketCreated(std::shared_ptr<ccWebsocket> newWebsocket)
{
    for (const auto& item : _aWSMs)
    {
        if (item->hasUri(newWebsocket->getUri()))
        {
            item->addWebsocket(newWebsocket);
            return;
        }
    }
}

void ccWebServerManager::onWebsocketConnected(std::int32_t socketID)
{
    doPerformWebsocketEvent(ccWebsocket::ccWebSocketEvent_Connected, socketID, _strNoData);
}

void ccWebServerManager::onWebsocketDisconnected(std::int32_t socketID)
{
    doPerformWebsocketEvent(ccWebsocket::ccWebSocketEvent_Disconnected, socketID, _strNoData);
}

void ccWebServerManager::onWebsocketReceivedData(std::int32_t socketID, const std::string& strData)
{
    doPerformWebsocketEvent(ccWebsocket::ccWebSocketEvent_ReceivedData, socketID, strData);
}

void ccWebServerManager::doPerformWebsocketEvent(ccWebsocket::ccWebSocketEvent eEvent, std::int32_t socketID, const std::string& strData)
{
    std::shared_ptr<ccWebsocket> newWebsocket;

    for (const auto& item : _aWSMs)
    {
        if (item->getWebsocket(socketID, newWebsocket))
        {
            if (newWebsocket != NULL)
                item->performWebsocketEvent(eEvent, newWebsocket, strData);

            return;
        }
    }
}

}
