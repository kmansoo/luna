/*
* ccWebServerEventListener.h
*
*  Created on: 2015. 11. 14.
*      Author: kmansoo
*/

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVEREVENTLISTENERH_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVEREVENTLISTENERH_

#include "ccWebServerRequest.h"
#include "ccWebServerResponse.h"
#include "ccWebsocket.h"

namespace Luna {

class ccWebServerEventListener
{
public:
    virtual bool onWebServerRequest(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response) = 0;

    //  return value
    //      true    :   accept
    //      false   :   reject
    virtual bool onNewWebsocketRequest(const std::string& strWebsocketUri) { return false; }
    virtual void onWebsocketCreated(std::shared_ptr<ccWebsocket> newWebsocket) {};
    virtual void onWebsocketConnected(std::int32_t socketID) {}
    virtual void onWebsocketReceivedData(std::int32_t socketID, const std::string& strData) {}
    virtual void onWebsocketDisconnected(std::int32_t socketID) {}
};

}

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVEREVENTLISTENERH_ */
