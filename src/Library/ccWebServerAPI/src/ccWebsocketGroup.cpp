#include "stdafx.h"
#include "ccWebsocketGroup.h"

#include <algorithm>

ccWebsocketGroup::ccWebsocketGroup(const std::string& strUri)
{
    _strUri = strUri;
}


ccWebsocketGroup::~ccWebsocketGroup()
{
}

const std::string& ccWebsocketGroup::GetUri()
{
    return _strUri;
}

bool ccWebsocketGroup::Add(std::shared_ptr<ccWebsocket> pNewWS)
{
    std::lock_guard<std::mutex> lock(_mtx);

    std::vector<std::shared_ptr<ccWebsocket>>::iterator it;
    
    if ((it = std::find(_aWSList.begin(), _aWSList.end(), pNewWS)) != _aWSList.end())
        return false;

    _aWSList.push_back(pNewWS);

    return true;
}

bool ccWebsocketGroup::Remove(std::shared_ptr<ccWebsocket> pNewWS)
{
    std::lock_guard<std::mutex> lock(_mtx);

    std::vector<std::shared_ptr<ccWebsocket>>::iterator it;

    if ((it = std::find(_aWSList.begin(), _aWSList.end(), pNewWS)) == _aWSList.end())
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

void  ccWebsocketGroup::Broadcast(const char* strMessage, std::size_t size)
{
    std::lock_guard<std::mutex> lock(_mtx);

    for (auto ws : _aWSList)
        ws->Send(strMessage, size);
}

void  ccWebsocketGroup::Broadcast(const std::string strMessage)
{
    Broadcast(strMessage.c_str(), strMessage.length());
}
