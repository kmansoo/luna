#include "stdafx.h"
#include "ccWebsocketGroupManager.h"

#include <algorithm>

ccWebsocketGroupManager::ccWebsocketGroupManager()
{
}


ccWebsocketGroupManager::~ccWebsocketGroupManager()
{
}

bool ccWebsocketGroupManager::Add(std::shared_ptr<ccWebsocket> pNewWS)
{
    std::lock_guard<std::mutex> lock(_mtx);

    std::vector<std::shared_ptr<ccWebsocketGroup>>::iterator it;
    
    if ((it = std::find(_aWSGList.begin(), _aWSGList.end(), pNewWS)) != _aWSGList.end())
        return false;

    _aWSGList.push_back(pNewWS);

    return true;
}

bool ccWebsocketGroupManager::Remove(std::shared_ptr<ccWebsocket> pNewWS)
{
    std::lock_guard<std::mutex> lock(_mtx);

    std::vector<std::shared_ptr<ccWebsocket>>::iterator it;

    if ((it = std::find(_aWSGList.begin(), _aWSGList.end(), pNewWS)) == _aWSGList.end())
        return false;

    _aWSGList.erase(it);

    return true;
}

bool ccWebsocketGroupManager::RemoveAll()
{
    std::lock_guard<std::mutex> lock(_mtx);

    _aWSList.clear();

    return true;

}

void  ccWebsocketGroupManager::Broadcast(const char* strMessage, std::size_t size)
{
    std::lock_guard<std::mutex> lock(_mtx);

    for (auto ws : _aWSList)
        ws->Send(strMessage, size);
}

void  ccWebsocketGroupManager::Broadcast(const std::string strMessage)
{
    Broadcast(strMessage.c_str(), strMessage.length());
}
