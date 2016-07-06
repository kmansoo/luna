/*
* ccEasyWebsocketClient.cpp
*
*  Created on: 2015. 12. 17
*      Author: kmansoo
*/

#ifndef FOSSLIBRARY_CCWEBSOCKETCLIENTAPI_CCEASYWEBSOCKETCLIENT_H_
#define FOSSLIBRARY_CCWEBSOCKETCLIENTAPI_CCEASYWEBSOCKETCLIENT_H_

#include <string>
#include <memory>
#include <thread>

#include "ccWebServer/ccWebsocket.h"

#include "easywsclient/easywsclient.hpp"

//  I will implement a wrapper class for easywsclient.

namespace Luna {

class ccEasyWebsocketClient : public ccWebsocket
{
public:
    ccEasyWebsocketClient();
    ccEasyWebsocketClient(const std::string& strUri);
    ~ccEasyWebsocketClient();

public:
    virtual bool    open(const std::string& strUri);
    virtual bool    close();

    virtual std::int32_t    getInstance();  // It may be a Socket ID. 
    virtual bool            send(const std::string& strMessage);
    virtual bool            sendBinary(const void* pBuffer, std::size_t size);

private:
    void    doPoll();
    void    doReceive(const std::string& message);

private:
    std::shared_ptr<easywsclient::WebSocket> _pWorkWS;
    std::thread             _oPollThread;
    bool                    _bIsStopThread;
    const std::string       _oNullData;
};

}

#endif
