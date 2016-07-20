#include "ccWebsocketGroup.h"

#include <algorithm>

namespace Luna {

ccWebsocketGroup::ccWebsocketGroup(const std::string& strUri)
{
    _strUri = strUri;
}

ccWebsocketGroup::~ccWebsocketGroup()
{
    removeAll();
}

const std::string& ccWebsocketGroup::getUri()
{
    return _strUri;
}

const std::size_t ccWebsocketGroup::getCount() const
{
    return _aWSList.size();
}

bool ccWebsocketGroup::add(std::shared_ptr<ccWebsocket> pNewWS)
{
    if (pNewWS == NULL)
        return false;

    std::lock_guard<std::mutex> lock(_mtx);

    _aWSList[pNewWS->getInstance()] = pNewWS;

    pNewWS->setGroup(this);

    return true;
}

bool ccWebsocketGroup::remove(std::shared_ptr<ccWebsocket> pNewWS)
{
    std::lock_guard<std::mutex> lock(_mtx);

    auto it = _aWSList.find(pNewWS->getInstance());

    if (it == _aWSList.end())
        return false;

    it->second->setGroup(NULL);

    _aWSList.erase(it);

    return true;
}

bool ccWebsocketGroup::removeAll()
{
    std::lock_guard<std::mutex> lock(_mtx);

    _aWSList.clear();

    return true;
}

bool ccWebsocketGroup::getWebsocket(std::int32_t nInstance, std::shared_ptr<ccWebsocket>& pSocket)
{
    std::lock_guard<std::mutex> lock(_mtx);

    auto item = _aWSList.find(nInstance);

    if (item == _aWSList.end())
        return false;

    pSocket = item->second;

    return true;
}

void  ccWebsocketGroup::broadcast(const std::string& strMessage)
{
    std::lock_guard<std::mutex> lock(_mtx);

    for (auto item : _aWSList)
        item.second->send(strMessage);
}

void ccWebsocketGroup::broadcastEx(const std::string& strMessage, std::shared_ptr<ccWebsocket>& pExceptSocket)
{
    std::lock_guard<std::mutex> lock(_mtx);

    for (auto item : _aWSList)
    {
        if (item.second != pExceptSocket)
            item.second->send(strMessage);
    }
}

}
