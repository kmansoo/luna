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

struct ccWebServerEventListener {
    virtual bool on_web_request(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response) = 0;

    //  return value
    //      true    :   accept
    //      false   :   reject
    virtual bool on_new_websocket_request(const std::string& websocketuri_) { return false; }
    virtual void on_websocket_created(std::shared_ptr<ccWebsocket> new_websocket) {};
    virtual void on_websocket_connected(std::int32_t socket_id) {}
    virtual void on_websocket_received_data(std::int32_t socket_id, const std::string& data) {}
    virtual void on_websocket_disconnected(std::int32_t socket_id) {}
};

}

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVEREVENTLISTENERH_ */
