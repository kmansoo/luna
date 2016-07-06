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
    virtual bool            Open(const std::string& strUri) = 0;
    virtual bool            Close() = 0;

    virtual std::int32_t    GetInstance() = 0;  // It may be a Socket ID. 
    virtual bool            Send(const std::string& strMessage) = 0;
    virtual bool            SendBinary(const void* pBuffer, std::size_t size) = 0;

public:
    void                    SetEventListener(WebSocketEventFunction f);

    const std::string       GetUri();
    void                    SetGroup(ccWebsocketGroup* pGroup);
    ccWebsocketGroup*       GetGroup();

protected:
    std::string             _strUri;
    ccWebsocketGroup*       _pMyGroup;
    WebSocketEventFunction  _oEventListener;
};

}

#endif
