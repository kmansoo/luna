#include "stdafx.h"
#include "ccWebsocketManager.h"

#include <algorithm>

ccWebsocketManager::ccWebsocketManager()
{
}


ccWebsocketManager::~ccWebsocketManager()
{
    RemoveAllWebsocket();
}

bool ccWebsocketManager::AddWebsocket(std::shared_ptr<ccWebsocket> pNewWS)
{
    if (!HasUri(pNewWS->GetUri()))
        return false;

    //  add new group
    auto it = _aWSGList.find(pNewWS->GetUri());

    if (it != _aWSGList.end())
        it->second->Add(pNewWS);
    else
    {
        std::shared_ptr<ccWebsocketGroup> newGroup(new ccWebsocketGroup(pNewWS->GetUri()));
        _aWSGList[pNewWS->GetUri()] = newGroup;

        newGroup->Add(pNewWS);
    }

    return true;
}

bool ccWebsocketManager::RemoveWebsocket(std::shared_ptr<ccWebsocket> pNewWS)
{
    std::lock_guard<std::mutex> lock(_mtx);

    auto it = _aWSGList.find(pNewWS->GetUri());

    if (it != _aWSGList.end())
        it->second->Remove(pNewWS);

    if (it->second->GetCount() == 0)
        _aWSGList.erase(it);

    return true;
}

bool ccWebsocketManager::RemoveAllWebsocket()
{
    std::lock_guard<std::mutex> lock(_mtx);

    for (auto item : _aWSGList)
        item.second->RemoveAll();

    _aWSGList.clear();

    return true;
}

bool ccWebsocketManager::GetWebsocket(std::int32_t nInstance, std::shared_ptr<ccWebsocket>& pWebsocket)
{
    std::lock_guard<std::mutex> lock(_mtx);

    for (auto item : _aWSGList)
    {
        if (item.second->GetWebsocket(nInstance, pWebsocket))
            return true;
    }

    return false;
}

bool ccWebsocketManager::GetWebsocket(const std::string& strUri, std::int32_t nInstance, std::shared_ptr<ccWebsocket>& pWebsocket)
{
    std::shared_ptr<ccWebsocketGroup> pGroup;

    if (GetGroup(strUri, pGroup) == false)
        return false;

    return pGroup->GetWebsocket(nInstance, pWebsocket);
}

bool ccWebsocketManager::GetGroup(const std::string& strUri, std::shared_ptr<ccWebsocketGroup>& pGroup)
{
    std::lock_guard<std::mutex> lock(_mtx);

    auto it = _aWSGList.find(strUri);

    if (it == _aWSGList.end())
        return false;

    pGroup = it->second;

    return true;
}

bool ccWebsocketManager::HasUri(const std::string& strUri)
{
    auto it = _aFunctions.find(strUri);

    if (it == _aFunctions.end())
        return false;

    return true;
}

bool ccWebsocketManager::AddFunction(const std::string& strUri, std::function<bool(ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<ccWebsocket> pWS, const std::string& strData)> f)
{
    _aFunctions[strUri] = f;

    return true;
}

bool ccWebsocketManager::RemoveFunction(const std::string& strUri)
{
    std::lock_guard<std::mutex> lock(_mtxFunction);

    auto it = _aFunctions.find(strUri);

    if (it == _aFunctions.end())
        return false;

    _aFunctions.erase(it);

    return true;
}

bool ccWebsocketManager::PerformWebsocketEvent(ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<ccWebsocket> pWS, const std::string& strData)
{
    std::lock_guard<std::mutex> lock(_mtxFunction);

    if (HasUri(pWS->GetUri()) == false)
        return false;

    _aFunctions[pWS->GetUri()](eEvent, pWS, strData);

    if (eEvent == ccWebsocket::ccWebSocketEvent_Disconnected)
        RemoveWebsocket(pWS);

    return true;
}
