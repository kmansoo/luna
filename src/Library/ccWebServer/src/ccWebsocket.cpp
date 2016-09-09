/*
* ccWebsocket.cpp
*
*  Created on: 2015. 12. 14.
*      Author: kmansoo
*/

#include "ccWebsocketGroup.h"

#include "ccWebsocket.h"

namespace Luna {

ccWebsocket::ccWebsocket(const std::string& uri)
    : my_group_(NULL) {
    uri_ = uri;
}

ccWebsocket::~ccWebsocket() {
}

void  ccWebsocket::set_event_listener(WebSocketEventFunction f) {
    event_listener_ = f;
}


const std::string ccWebsocket::get_uri() {
    return uri_;
}

void ccWebsocket::set_group(ccWebsocketGroup* group) {
    my_group_ = group;
}

ccWebsocketGroup* ccWebsocket::get_group() {
    return my_group_;
}

bool ccWebsocket::send(const std::string& strMessage) {
    return false;
}

}
