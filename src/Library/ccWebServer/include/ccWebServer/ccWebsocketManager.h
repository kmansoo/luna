/*
* ccWebsocketManager.h
*
*  Created on: 2015. 12. 14.
*      Author: kmansoo
*/

#pragma once

#include <string>
#include <map>
#include <memory>
#include <mutex>
#include <functional>

#include "ccWebsocketGroup.h"

namespace Luna {

class ccWebsocketManager {
public:
    ccWebsocketManager();
    virtual ~ccWebsocketManager();

public:
    bool    add_websocket(std::shared_ptr<ccWebsocket> new_websocket);
    bool    remove_websocket(std::shared_ptr<ccWebsocket> new_websocket);
    bool    remove_all_websocket();

    bool    get_websocket(std::int32_t instance, std::shared_ptr<ccWebsocket>& websocket);
    bool    get_websocket(const std::string& uri, std::int32_t instance, std::shared_ptr<ccWebsocket>& websocket);
    bool    get_websocket(void* connection_info, std::shared_ptr<ccWebsocket>& websocket);
    bool    get_group(const std::string& uri, std::shared_ptr<ccWebsocketGroup>& group);

public:
    virtual bool    has_uri(const std::string& uri);
    virtual bool    add_function(const std::string& uri, std::function<bool(ccWebsocket::ccWebSocketEvent ws_event, std::shared_ptr<ccWebsocket> websocket, const std::string& data)> f);
    virtual bool    remove_function(const std::string& uri);
    virtual bool    perform_websocket_event(ccWebsocket::ccWebSocketEvent ws_event, std::shared_ptr<ccWebsocket> websocket, const std::string& data);

protected:
    std::mutex      mtx_;
    std::mutex      mtx_Function;

    std::map<std::string, std::shared_ptr<ccWebsocketGroup>>    websocket_group_map_;
    std::map<std::string, std::function<bool(ccWebsocket::ccWebSocketEvent ws_event, std::shared_ptr<ccWebsocket> websocket, const std::string& data)>> function_map_;
};

}
