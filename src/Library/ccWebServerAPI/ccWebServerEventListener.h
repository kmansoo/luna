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

class ccWebServerEventListener
{
public:
    virtual bool OnWebServerRequest(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response) = 0;

    //  return value
    //      true    :   accept
    //      false   :   reject
    virtual bool OnNewWebsocketRequest(std::shared_ptr<ccWebsocket> newWebsocket) { return false; }
    virtual void OnWebsocketConnected(std::shared_ptr<ccWebsocket> newWebsocket) {}
    virtual void OnWebsocketData(std::shared_ptr<ccWebsocket> newWebsocket) {}
    virtual void OnWebsocketClosed(std::shared_ptr<ccWebsocket> newWebsocket) {}
};

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVEREVENTLISTENERH_ */
