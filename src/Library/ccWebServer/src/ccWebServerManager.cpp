/*
 * ccWebServerManager.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */
#include <iostream>
#include <algorithm>

#include "ccWebServer/ccWebServerManager.h"

namespace Luna {

ccWebServerManager::ccWebServerManager() {
    // TODO Auto-generated constructor stub

}

ccWebServerManager::~ccWebServerManager() {
    // TODO Auto-generated destructor stub
}

void  ccWebServerManager::attach_factory(std::shared_ptr<ccWebServerObjectFactory> factory) {
    obj_factory_ = factory;
}

bool ccWebServerManager::create_web_server(
    const std::string& name,
    const std::string& ports,
    std::shared_ptr<ccWebServerPageDirectory> page_directory) {

    if (obj_factory_ == nullptr)
        return false;

    web_server_list_.push_back(
        obj_factory_->create_web_server(name, ports, ".", page_directory));

    return true;
}

bool ccWebServerManager::create_web_server(
    const std::string& name,
    const std::string& ports,
    const std::string& root_path,
    std::shared_ptr<ccWebServerPageDirectory> page_directory) {

    if (obj_factory_ == nullptr)
        return false;

    web_server_list_.push_back(
        obj_factory_->create_web_server(name, ports, root_path, page_directory));

    return true;
}

std::shared_ptr<ccWebServer> ccWebServerManager::get_web_server(const char* strName) {
    for (auto server : web_server_list_) {
        if (server->get_name().compare(strName) == 0)
            return server;
    }

    return nullptr;
}

void ccWebServerManager::start() {
    for (auto server : web_server_list_) {
        server->set_listener(this);
        server->start();
    }
}

void ccWebServerManager::stop() {
    for (const auto server : web_server_list_) {
        server->set_listener(NULL);
        server->stop();
    }
}

bool ccWebServerManager::add_restful_api(std::shared_ptr<ccRESTfulApi> new_api) {
    for (auto item : web_api_list_) {
        if (item == new_api)
            return false;
    }

    web_api_list_.push_back(new_api);

    return true;
}

bool ccWebServerManager::remove_restful_api(std::shared_ptr<ccRESTfulApi> new_api) {
    auto it = std::find(std::begin(web_api_list_), std::end(web_api_list_), new_api);

    if (it != std::end(web_api_list_)) {
        web_api_list_.erase(it);

        return true;
    }

    return false;
}

bool ccWebServerManager::remove_all_restful_api() {
    web_api_list_.clear();

    return true;
}

bool ccWebServerManager::add_websocket_manager(std::shared_ptr<ccWebsocketManager> new_websocket_manager) {
    for (const auto& item : websocket_manager_list_) {
        if (item == new_websocket_manager)
            return false;
    }

    websocket_manager_list_.push_back(new_websocket_manager);

    return true;
}


bool ccWebServerManager::remove_websocket_manager(std::shared_ptr<ccWebsocketManager> new_websocket_manager) {
    auto it = std::find(std::begin(websocket_manager_list_), std::end(websocket_manager_list_), new_websocket_manager);

    if (it != std::end(websocket_manager_list_)) {
        websocket_manager_list_.erase(it);

        return true;
    }

    return false;
}

bool ccWebServerManager::remove_all_websocket_manager() {
    websocket_manager_list_.clear();

    return true;
}

bool ccWebServerManager::on_web_request(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response) {
    for (const auto& api : web_api_list_) {
        if (api->has_api(request->geturi_()) == true)
            return api->perform_api(request, response);
    }

    return false;
}

bool ccWebServerManager::on_new_websocket_request(const std::string& websocketuri_) {
    if (websocket_manager_list_.size() == 0)
        return false;

    for (const auto& item : websocket_manager_list_) {
        if (item->has_uri(websocketuri_))
            return true;
    }

    return false;
}

void ccWebServerManager::on_websocket_created(std::shared_ptr<ccWebsocket> new_websocket) {
    for (const auto& item : websocket_manager_list_) {
        if (item->has_uri(new_websocket->get_uri())) {
            item->add_websocket(new_websocket);
            return;
        }
    }
}

void ccWebServerManager::on_websocket_connected(std::int32_t socket_id) {
    do_perform_websocket_event(ccWebsocket::ccWebSocketEvent_Connected, socket_id, blank_string_);
}

void ccWebServerManager::on_websocket_disconnected(std::int32_t socket_id) {
    do_perform_websocket_event(ccWebsocket::ccWebSocketEvent_Disconnected, socket_id, blank_string_);
}

int  ccWebServerManager::on_websocket_check_instance(void* connection_info) {
    std::shared_ptr<ccWebsocket> websocket;

    for (const auto& item : websocket_manager_list_) {
        if (item->get_websocket(connection_info, websocket)) {
            return websocket->get_instance();
        }
    }

    return -1;
}

void ccWebServerManager::on_websocket_received_data(std::int32_t socket_id, const std::string& data) {
    do_perform_websocket_event(ccWebsocket::ccWebSocketEvent_ReceivedData, socket_id, data);
}

void ccWebServerManager::do_perform_websocket_event(ccWebsocket::ccWebSocketEvent event, std::int32_t socket_id, const std::string& data) {
    std::shared_ptr<ccWebsocket> websocket;

    for (const auto& item : websocket_manager_list_) {
        if (item->get_websocket(socket_id, websocket)) {
            if (websocket != NULL)
                item->perform_websocket_event(event, websocket, data);

            return;
        }
    }
}

}
