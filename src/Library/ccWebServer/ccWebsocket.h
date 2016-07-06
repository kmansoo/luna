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

class ccWebsocket
{
public:
    ccWebsocket() {};
    ccWebsocket(const std::string& strUri);
    virtual ~ccWebsocket();

public:
    enum ccWebSocketEvent {
        ccWebSocketEvent_Connected,
        ccWebSocketEvent_Disconnected,
        ccWebSocketEvent_ReceivedData,
    };

public:
    typedef std::function<void(ccWebSocketEvent eEvent, const std::string& message)> WebSocketEventFunction;

public:
    virtual bool            open(const std::string& strUri) = 0;
    virtual bool            close() = 0;

    virtual std::int32_t    getInstance() = 0;  // It may be a Socket ID. 
    virtual bool            send(const std::string& strMessage) = 0;
    virtual bool            sendBinary(const void* pBuffer, std::size_t size) = 0;

public:
    void                    setEventListener(WebSocketEventFunction f);

    const std::string       getUri();
    void                    setGroup(ccWebsocketGroup* pGroup);
    ccWebsocketGroup*       getGroup();

protected:
    std::string             _strUri;
    ccWebsocketGroup*       _pMyGroup;
    WebSocketEventFunction  _oEventListener;
};

}

#endif
