#include "stdafx.h"
#include "ccWebsocketGroupManager.h"

#include <algorithm>

ccWebsocketGroupManager::ccWebsocketGroupManager()
{
}


ccWebsocketGroupManager::~ccWebsocketGroupManager()
{
    RemoveAll();
}

bool ccWebsocketGroupManager::Add(std::shared_ptr<ccWebsocket> pNewWS)
{
    std::lock_guard<std::mutex> lock(_mtx);

    std::map<std::string, std::shared_ptr<ccWebsocketGroup>>::iterator it;

    it = _aWSGList.find(pNewWS->GetUri());

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

bool ccWebsocketGroupManager::Remove(std::shared_ptr<ccWebsocket> pNewWS)
{
    std::lock_guard<std::mutex> lock(_mtx);

    std::map<std::string, std::shared_ptr<ccWebsocketGroup>>::iterator it;

    it = _aWSGList.find(pNewWS->GetUri());

    if (it != _aWSGList.end())
        it->second->Remove(pNewWS);

    if (it->second->GetCount() == 0)
        _aWSGList.erase(it);

    return true;
}

bool ccWebsocketGroupManager::RemoveAll()
{
    std::lock_guard<std::mutex> lock(_mtx);

    for (auto item : _aWSGList)
        item.second->RemoveAll();

    _aWSGList.clear();

    return true;
}

bool ccWebsocketGroupManager::GetWebsocket(std::int32_t nInstance, std::shared_ptr<ccWebsocket>& pWebsocket)
{
    std::lock_guard<std::mutex> lock(_mtx);

    for (auto item : _aWSGList)
    {
        if (item.second->GetWebsocket(nInstance, pWebsocket))
            return true;
    }

    return false;
}

bool ccWebsocketGroupManager::GetWebsocket(const std::string& strUri, std::int32_t nInstance, std::shared_ptr<ccWebsocket>& pWebsocket)
{
    std::shared_ptr<ccWebsocketGroup> pGroup;

    if (GetGroup(strUri, pGroup) == false)
        return false;

    return pGroup->GetWebsocket(nInstance, pWebsocket);
}

bool ccWebsocketGroupManager::GetGroup(const std::string& strUri, std::shared_ptr<ccWebsocketGroup>& pGroup)
{
    std::lock_guard<std::mutex> lock(_mtx);

    std::map<std::string, std::shared_ptr<ccWebsocketGroup>>::iterator it;

    it = _aWSGList.find(strUri);

    if (it == _aWSGList.end())
        return false;

    pGroup = it->second;

    return true;
}
