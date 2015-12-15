#include "stdafx.h"
#include "ccWebsocketGroup.h"

#include <algorithm>

ccWebsocketGroup::ccWebsocketGroup(const std::string& strUri)
{
    _strUri = strUri;
}

ccWebsocketGroup::ccWebsocketGroup(const char* pUri, std::size_t size)
{
    _strUri.assign(pUri, size);
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

    std::map< std::int32_t, std::shared_ptr<ccWebsocket> >::iterator it;

    it = _aWSList.find(pNewWS->GetInstance());

    if (it != _aWSList.end())
        _aWSList.erase(it);

    _aWSList[pNewWS->GetInstance()] = pNewWS;

    return true;
}

bool ccWebsocketGroup::Remove(std::shared_ptr<ccWebsocket> pNewWS)
{
    std::lock_guard<std::mutex> lock(_mtx);

    std::map< std::int32_t, std::shared_ptr<ccWebsocket> >::iterator it;

    it = _aWSList.find(pNewWS->GetInstance());

    if (it == _aWSList.end())
        return false;

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

    std::map< std::int32_t, std::shared_ptr<ccWebsocket> >::iterator it;

    it = _aWSList.find(nInstance);

    if (it == _aWSList.end())
        return false;

    pSocket = it->second;

    return true;
}

void  ccWebsocketGroup::Broadcast(const char* strMessage, std::size_t size)
{
    std::lock_guard<std::mutex> lock(_mtx);

    std::map< std::int32_t, std::shared_ptr<ccWebsocket> >::iterator it;

    for (auto item : _aWSList)
        item.second->Send(strMessage, size);
}

void  ccWebsocketGroup::Broadcast(const std::string strMessage)
{
    Broadcast(strMessage.c_str(), strMessage.length());
}
