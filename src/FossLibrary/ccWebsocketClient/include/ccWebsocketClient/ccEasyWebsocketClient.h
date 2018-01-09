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

class ccEasyWebsocketClient : public ccWebsocket {
public:
    ccEasyWebsocketClient();
    ccEasyWebsocketClient(const std::string& uri);
    ~ccEasyWebsocketClient();

public:
    virtual bool    open(const std::string& uri);
    virtual bool    close();

    virtual std::int32_t    get_instance();  // It may be a Socket ID. 
    virtual void*           get_connection_info() { return NULL; }
    virtual bool            send(const std::string& message);
    virtual bool            send_binary(const void* buffer, std::size_t size);

private:
    void    poll();
    void    receive(const std::string& message);

private:
    std::shared_ptr<easywsclient::WebSocket> websocket_;
    std::thread             polling_thread_;
    bool                    is_stop_thread_;
    const std::string       blank_string_data_;
};

}

#endif
