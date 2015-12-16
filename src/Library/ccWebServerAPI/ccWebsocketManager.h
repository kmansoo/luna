/*
* ccWebsocketManager.h
*
*  Created on: 2015. 12. 14.
*      Author: kmansoo
*/

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSOCKET_MANAGER_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSOCKET_MANAGER_H_

#include <string>
#include <map>
#include <memory>
#include <mutex>
#include <functional>

#include "ccWebsocketGroup.h"

class ccWebsocketManager
{
public:
    ccWebsocketManager();
    virtual ~ccWebsocketManager();

public:
    bool    AddWebsocket(std::shared_ptr<ccWebsocket> pNewWS);
    bool    RemoveWebsocket(std::shared_ptr<ccWebsocket> pNewWS);
    bool    RemoveAllWebsocket();

    bool    GetWebsocket(std::int32_t nInstance, std::shared_ptr<ccWebsocket>& pSocket);
    bool    GetWebsocket(const std::string& strUri, std::int32_t nInstance, std::shared_ptr<ccWebsocket>& pSocket);
    bool    GetGroup(const std::string& strUri, std::shared_ptr<ccWebsocketGroup>& pGroup);

public:
    virtual bool    HasUri(const std::string& strUri);
    virtual bool    AddFunction(const std::string& strUri, std::function<bool(ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<ccWebsocket> pWS, const char* strData, std::size_t size)> f);
    virtual bool    PerformWebsocketEvent(ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<ccWebsocket> pWS, const char* strData, std::size_t size);

protected:
    std::mutex      _mtx;

    std::map<std::string, std::shared_ptr<ccWebsocketGroup>>    _aWSGList;
    std::map<std::string, std::function<bool(ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<ccWebsocket> pWS, const char* strData, std::size_t size)>> _aFunctions;
};

#endif
