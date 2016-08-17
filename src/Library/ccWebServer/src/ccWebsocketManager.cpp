#include "ccWebsocketManager.h"

#include <algorithm>

namespace Luna {

ccWebsocketManager::ccWebsocketManager()
{
}

ccWebsocketManager::~ccWebsocketManager()
{
    removeAllWebsocket();
}

bool ccWebsocketManager::addWebsocket(std::shared_ptr<ccWebsocket> pNewWS)
{
    if (!hasUri(pNewWS->getUri()))
        return false;

    //  add new group
    auto it = _aWSGList.find(pNewWS->getUri());

    if (it != _aWSGList.end())
        it->second->add(pNewWS);
    else
    {
        auto newGroup = std::make_shared<ccWebsocketGroup>(pNewWS->getUri());
        _aWSGList[pNewWS->getUri()] = newGroup;

        newGroup->add(pNewWS);
    }

    return true;
}

bool ccWebsocketManager::removeWebsocket(std::shared_ptr<ccWebsocket> pNewWS)
{
    std::lock_guard<std::mutex> lock(_mtx);

    auto it = _aWSGList.find(pNewWS->getUri());

    if (it != _aWSGList.end())
        it->second->remove(pNewWS);

    if (it->second->getCount() == 0)
        _aWSGList.erase(it);

    return true;
}

bool ccWebsocketManager::removeAllWebsocket()
{
    std::lock_guard<std::mutex> lock(_mtx);

    for (auto item : _aWSGList)
        item.second->removeAll();

    _aWSGList.clear();

    return true;
}

bool ccWebsocketManager::getWebsocket(std::int32_t nInstance, std::shared_ptr<ccWebsocket>& pWebsocket)
{
    std::lock_guard<std::mutex> lock(_mtx);

    for (auto item : _aWSGList)
    {
        if (item.second->getWebsocket(nInstance, pWebsocket))
            return true;
    }

    return false;
}

bool ccWebsocketManager::getWebsocket(const std::string& strUri, std::int32_t nInstance, std::shared_ptr<ccWebsocket>& pWebsocket)
{
    std::shared_ptr<ccWebsocketGroup> pGroup;

    if (getGroup(strUri, pGroup) == false)
        return false;

    return pGroup->getWebsocket(nInstance, pWebsocket);
}

bool ccWebsocketManager::getGroup(const std::string& strUri, std::shared_ptr<ccWebsocketGroup>& pGroup)
{
    std::lock_guard<std::mutex> lock(_mtx);

    auto it = _aWSGList.find(strUri);

    if (it == _aWSGList.end())
        return false;

    pGroup = it->second;

    return true;
}

bool ccWebsocketManager::hasUri(const std::string& strUri)
{
    auto it = _aFunctions.find(strUri);

    if (it == _aFunctions.end())
        return false;

    return true;
}

bool ccWebsocketManager::addFunction(const std::string& strUri, std::function<bool(ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<ccWebsocket> pWS, const std::string& strData)> f)
{
    _aFunctions[strUri] = f;

    return true;
}

bool ccWebsocketManager::removeFunction(const std::string& strUri)
{
    std::lock_guard<std::mutex> lock(_mtxFunction);

    auto it = _aFunctions.find(strUri);

    if (it == _aFunctions.end())
        return false;

    _aFunctions.erase(it);

    return true;
}

bool ccWebsocketManager::performWebsocketEvent(ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<ccWebsocket> pWS, const std::string& strData)
{
    std::lock_guard<std::mutex> lock(_mtxFunction);

    if (hasUri(pWS->getUri()) == false)
        return false;

    _aFunctions[pWS->getUri()](eEvent, pWS, strData);

    if (eEvent == ccWebsocket::ccWebSocketEvent_Disconnected)
        removeWebsocket(pWS);

    return true;
}

}
