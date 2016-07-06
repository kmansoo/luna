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
    virtual bool OnWebServerRequest(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response) = 0;

    //  return value
    //      true    :   accept
    //      false   :   reject
    virtual bool OnNewWebsocketRequest(const std::string& strWebsocketUri) { return false; }
    virtual void OnWebsocketCreated(std::shared_ptr<ccWebsocket> newWebsocket) {};
    virtual void OnWebsocketConnected(std::int32_t socketID) {}
    virtual void OnWebsocketReceivedData(std::int32_t socketID, const std::string& strData) {}
    virtual void OnWebsocketDisconnected(std::int32_t socketID) {}
};

}

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVEREVENTLISTENERH_ */
