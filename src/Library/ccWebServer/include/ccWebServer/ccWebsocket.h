/*
* ccWebsocket.h
*
*  Created on: 2015. 12. 14.
*      Author: kmansoo
*/

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSOCKET_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSOCKET_H_

#include <string>
#include <functional>

namespace Luna {

class ccWebsocketGroup;

class ccWebsocket {
public:
    ccWebsocket() {};
    ccWebsocket(const std::string& uri);
    virtual ~ccWebsocket();

public:
    enum ccWebSocketEvent {
        ccWebSocketEvent_Connected,
        ccWebSocketEvent_Disconnected,
        ccWebSocketEvent_ReceivedData,
    };

public:
    typedef std::function<void(ccWebSocketEvent event, const std::string& message)> WebSocketEventFunction;

public:
    virtual bool            open(const std::string& uri) = 0;
    virtual bool            close() = 0;

    virtual std::int32_t    get_instance() = 0;  // It may be a Socket ID. 
    virtual void*           get_connection_info() = 0;

    virtual bool            send(const std::string& message) = 0;
    virtual bool            send_binary(const void* pBuffer, std::size_t size) = 0;

public:
    void                    set_event_listener(WebSocketEventFunction f);

    const std::string       get_uri();
    void                    set_group(ccWebsocketGroup* group);
    ccWebsocketGroup*       get_group();

protected:
    std::string             uri_;
    ccWebsocketGroup*       my_group_;
    WebSocketEventFunction  event_listener_;
};

}

#endif
