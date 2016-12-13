#include <algorithm>

#include "ccWebServer/ccWebsocketManager.h"

namespace Luna {

ccWebsocketManager::ccWebsocketManager() {
}

ccWebsocketManager::~ccWebsocketManager() {
    remove_all_websocket();
}

bool ccWebsocketManager::add_websocket(std::shared_ptr<ccWebsocket> new_websocket) {
    if (!has_uri(new_websocket->get_uri()))
        return false;

    //  add new group
    auto it = websocket_group_map_.find(new_websocket->get_uri());

    if (it != websocket_group_map_.end())
        it->second->add(new_websocket);
    else {
        auto newGroup = std::make_shared<ccWebsocketGroup>(new_websocket->get_uri());
        websocket_group_map_[new_websocket->get_uri()] = newGroup;

        newGroup->add(new_websocket);
    }

    return true;
}

bool ccWebsocketManager::remove_websocket(std::shared_ptr<ccWebsocket> new_websocket) {
    std::lock_guard<std::mutex> lock(mtx_);

    auto it = websocket_group_map_.find(new_websocket->get_uri());

    if (it != websocket_group_map_.end())
        it->second->remove(new_websocket);

    if (it->second->get_count() == 0)
        websocket_group_map_.erase(it);

    return true;
}

bool ccWebsocketManager::remove_all_websocket() {
    std::lock_guard<std::mutex> lock(mtx_);

    for (auto item : websocket_group_map_)
        item.second->remove_all();

    websocket_group_map_.clear();

    return true;
}

bool ccWebsocketManager::get_websocket(std::int32_t instance, std::shared_ptr<ccWebsocket>& pWebsocket) {
    std::lock_guard<std::mutex> lock(mtx_);

    for (auto item : websocket_group_map_) {
        if (item.second->get_websocket(instance, pWebsocket))
            return true;
    }

    return false;
}

bool ccWebsocketManager::get_websocket(const std::string& uri, std::int32_t instance, std::shared_ptr<ccWebsocket>& pWebsocket) {
    std::shared_ptr<ccWebsocketGroup> group;

    if (get_group(uri, group) == false)
        return false;

    return group->get_websocket(instance, pWebsocket);
}

bool ccWebsocketManager::get_group(const std::string& uri, std::shared_ptr<ccWebsocketGroup>& group) {
    std::lock_guard<std::mutex> lock(mtx_);

    auto it = websocket_group_map_.find(uri);

    if (it == websocket_group_map_.end())
        return false;

    group = it->second;

    return true;
}

bool ccWebsocketManager::has_uri(const std::string& uri) {
    auto it = function_map_.find(uri);

    if (it == function_map_.end())
        return false;

    return true;
}

bool ccWebsocketManager::add_function(const std::string& uri, std::function<bool(ccWebsocket::ccWebSocketEvent event, std::shared_ptr<ccWebsocket> websocket, const std::string& data)> f) {
    function_map_[uri] = f;

    return true;
}

bool ccWebsocketManager::remove_function(const std::string& uri) {
    std::lock_guard<std::mutex> lock(mtx_Function);

    auto it = function_map_.find(uri);

    if (it == function_map_.end())
        return false;

    function_map_.erase(it);

    return true;
}

bool ccWebsocketManager::perform_websocket_event(ccWebsocket::ccWebSocketEvent event, std::shared_ptr<ccWebsocket> websocket, const std::string& data) {
    std::lock_guard<std::mutex> lock(mtx_Function);

    if (has_uri(websocket->get_uri()) == false)
        return false;

    function_map_[websocket->get_uri()](event, websocket, data);

    if (event == ccWebsocket::ccWebSocketEvent_Disconnected)
        remove_websocket(websocket);

    return true;
}

}
