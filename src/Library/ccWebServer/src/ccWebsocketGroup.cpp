#include <algorithm>

#include "ccWebServer/ccWebsocketGroup.h"

namespace Luna {

ccWebsocketGroup::ccWebsocketGroup(const std::string& uri) {
    uri_ = uri;
}

ccWebsocketGroup::~ccWebsocketGroup() {
    remove_all();
}

const std::string& ccWebsocketGroup::get_uri() {
    return uri_;
}

const std::size_t ccWebsocketGroup::get_count() const {
    return websocket_list_.size();
}

bool ccWebsocketGroup::add(std::shared_ptr<ccWebsocket> new_websocket) {
    if (new_websocket == NULL)
        return false;

    std::lock_guard<std::mutex> lock(mtx_);

    websocket_list_[new_websocket->get_instance()] = new_websocket;

    new_websocket->set_group(this);

    return true;
}

bool ccWebsocketGroup::remove(std::shared_ptr<ccWebsocket> new_websocket) {
    std::lock_guard<std::mutex> lock(mtx_);

    auto it = websocket_list_.find(new_websocket->get_instance());

    if (it == websocket_list_.end())
        return false;

    it->second->set_group(NULL);

    websocket_list_.erase(it);

    return true;
}

bool ccWebsocketGroup::remove_all() {
    std::lock_guard<std::mutex> lock(mtx_);

    websocket_list_.clear();

    return true;
}

bool ccWebsocketGroup::get_websocket(std::int32_t instance, std::shared_ptr<ccWebsocket>& socket) {
    std::lock_guard<std::mutex> lock(mtx_);

    auto item = websocket_list_.find(instance);

    if (item == websocket_list_.end())
        return false;

    socket = item->second;

    return true;
}

void  ccWebsocketGroup::broadcast(const std::string& strMessage) {
    std::lock_guard<std::mutex> lock(mtx_);

    for (auto item : websocket_list_)
        item.second->send(strMessage);
}

void ccWebsocketGroup::broadcast_ex(const std::string& strMessage, std::shared_ptr<ccWebsocket>& excepted_socket) {
    std::lock_guard<std::mutex> lock(mtx_);

    for (auto item : websocket_list_) {
        if (item.second != excepted_socket)
            item.second->send(strMessage);
    }
}

}
