#include "stdafx.h"
#include "ccWebsocketGroup.h"

#include <algorithm>

namespace Luna {

ccWebsocketGroup::ccWebsocketGroup(const std::string& strUri)
{
    _strUri = strUri;
}

ccWebsocketGroup::~ccWebsocketGroup()
{
    RemoveAll();
}

const std::string& ccWebsocketGroup::GetUri()
{
    return _strUri;
}

const std::size_t ccWebsocketGroup::GetCount() const
{
    return _aWSList.size();
}

bool ccWebsocketGroup::Add(std::shared_ptr<ccWebsocket> pNewWS)
{
    if (pNewWS == NULL)
        return false;

    std::lock_guard<std::mutex> lock(_mtx);

    _aWSList[pNewWS->GetInstance()] = pNewWS;

    pNewWS->SetGroup(this);

    return true;
}

bool ccWebsocketGroup::Remove(std::shared_ptr<ccWebsocket> pNewWS)
{
    std::lock_guard<std::mutex> lock(_mtx);

    auto it = _aWSList.find(pNewWS->GetInstance());

    if (it == _aWSList.end())
        return false;

    it->second->SetGroup(NULL);

    _aWSList.erase(it);

    return true;
}

bool ccWebsocketGroup::RemoveAll()
{
    std::lock_guard<std::mutex> lock(_mtx);

    _aWSList.clear();

    return true;
}

bool ccWebsocketGroup::GetWebsocket(std::int32_t nInstance, std::shared_ptr<ccWebsocket>& pSocket)
{
    std::lock_guard<std::mutex> lock(_mtx);

    auto item = _aWSList.find(nInstance);

    if (item == _aWSList.end())
        return false;

    pSocket = item->second;

    return true;
}

void  ccWebsocketGroup::Broadcast(const std::string& strMessage)
{
    std::lock_guard<std::mutex> lock(_mtx);

    for (auto item : _aWSList)
        item.second->Send(strMessage);
}

void ccWebsocketGroup::BroadcastEx(const std::string& strMessage, std::shared_ptr<ccWebsocket>& pExceptSocket)
{
    std::lock_guard<std::mutex> lock(_mtx);

    for (auto item : _aWSList)
    {
        if (item.second != pExceptSocket)
            item.second->Send(strMessage);
    }
}

}
