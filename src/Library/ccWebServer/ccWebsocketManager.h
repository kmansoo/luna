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

namespace Luna {

class ccWebsocketManager
{
public:
    ccWebsocketManager();
    virtual ~ccWebsocketManager();

public:
    bool    addWebsocket(std::shared_ptr<ccWebsocket> pNewWS);
    bool    removeWebsocket(std::shared_ptr<ccWebsocket> pNewWS);
    bool    removeAllWebsocket();

    bool    getWebsocket(std::int32_t nInstance, std::shared_ptr<ccWebsocket>& pSocket);
    bool    getWebsocket(const std::string& strUri, std::int32_t nInstance, std::shared_ptr<ccWebsocket>& pSocket);
    bool    getGroup(const std::string& strUri, std::shared_ptr<ccWebsocketGroup>& pGroup);

public:
    virtual bool    hasUri(const std::string& strUri);
    virtual bool    addFunction(const std::string& strUri, std::function<bool(ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<ccWebsocket> pWS, const std::string& strData)> f);
    virtual bool    removeFunction(const std::string& strUri);
    virtual bool    performWebsocketEvent(ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<ccWebsocket> pWS, const std::string& strData);

protected:
    std::mutex      _mtx;
    std::mutex      _mtxFunction;

    std::map<std::string, std::shared_ptr<ccWebsocketGroup>>    _aWSGList;
    std::map<std::string, std::function<bool(ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<ccWebsocket> pWS, const std::string& strData)>> _aFunctions;
};

}

#endif
