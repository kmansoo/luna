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

bool ccWebsocketGroup::get_websocket(std::int32_t instance, std::shared_ptr<ccWebsocket>& websocket) {
    std::lock_guard<std::mutex> lock(mtx_);

    auto item = websocket_list_.find(instance);

    if (item == websocket_list_.end())
        return false;

    websocket = item->second;

    return true;
}

bool ccWebsocketGroup::get_websocket(void* connection_info, std::shared_ptr<ccWebsocket>& websocket) {
    std::lock_guard<std::mutex> lock(mtx_);

    websocket = NULL;

    for (auto item : websocket_list_) {
        if (item.second->get_connection_info() == connection_info) {
            websocket = item.second;
            break;
        }
    }

    if (websocket == NULL)
        return false;

    return true;
}

void  ccWebsocketGroup::broadcast(const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx_);

    for (auto item : websocket_list_)
        item.second->send(message);
}

void ccWebsocketGroup::broadcast_ex(const std::string& message, std::shared_ptr<ccWebsocket>& excepted_websocket) {
    std::lock_guard<std::mutex> lock(mtx_);

    for (auto item : websocket_list_) {
        if (item.second != excepted_websocket)
            item.second->send(message);
    }
}

}
