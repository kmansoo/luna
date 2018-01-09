/*
* ccWebsocketGroup.h
*
*  Created on: 2015. 12. 14.
*      Author: kmansoo
*/

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSOCKETGROUP_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSOCKETGROUP_H_

#include <string>
#include <map>
#include <memory>
#include <mutex>

#include "ccWebsocket.h"

namespace Luna {

class ccWebsocketGroup {
public:
    ccWebsocketGroup(const std::string& uri);

    virtual ~ccWebsocketGroup();

public:
    const std::string&  get_uri();
    const std::size_t   get_count() const;

    bool    add(std::shared_ptr<ccWebsocket> new_websocket);
    bool    remove(std::shared_ptr<ccWebsocket> new_websocket);
    bool    remove_all();

    bool    get_websocket(std::int32_t instance, std::shared_ptr<ccWebsocket>& websocket);
    bool    get_websocket(void* connection_info, std::shared_ptr<ccWebsocket>& websocket);

    void    broadcast(const std::string& message);
    void    broadcast_ex(const std::string& message, std::shared_ptr<ccWebsocket>& excepted_websocket);

private:
    std::mutex      mtx_;
    std::string     uri_;

    std::map< std::int32_t, std::shared_ptr<ccWebsocket> >  websocket_list_;
};

}

#endif
