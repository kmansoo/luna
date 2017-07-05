/*
 * ccRESTfulWebServer.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include <iostream>
#include <functional>

#include "json/value.h"
#include "json/writer.h"
#include "json/reader.h"

#include "ccWebServer/ccWebServerManager.h"

#include "ccMongooseServer/ccMongooseWebServerObjectFactory.h"

#include "ccIoTDevice/ccIoTDeviceManager.h"

using namespace Luna;

ccIoTDeviceManager::ccIoTDeviceManager() {
    restful_api_.reset(new ccRESTfulApi);
    websocket_manager_.reset(new ccWebsocketManager);

    //  add commands
    command_map_["Register"] = std::bind(&ccIoTDeviceManager::register_command, this, std::placeholders::_1, std::placeholders::_2);
    command_map_["DeRegister"] = std::bind(&ccIoTDeviceManager::deregister_command, this, std::placeholders::_1, std::placeholders::_2);
    command_map_["GetDeviceStatus"] = std::bind(&ccIoTDeviceManager::get_device_status_command, this, std::placeholders::_1, std::placeholders::_2);
    command_map_["UpdateDeviceStatus"] = std::bind(&ccIoTDeviceManager::update_device_status_command, this, std::placeholders::_1, std::placeholders::_2);
    command_map_["SetControl"] = std::bind(&ccIoTDeviceManager::set_control_command, this, std::placeholders::_1, std::placeholders::_2);

    // TODO Auto-generated constructor stub
    restful_api_->addAPI(std::string("/devices"), std::bind(&ccIoTDeviceManager::devices, this, std::placeholders::_1, std::placeholders::_2));
    restful_api_->addAPI(std::string("/devices/device"), std::bind(&ccIoTDeviceManager::devices_device, this, std::placeholders::_1, std::placeholders::_2));

    websocket_manager_->add_function("/ws_iot_deivce", std::bind(&ccIoTDeviceManager::ws_iot_device, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    ccWebServerManager::instance().attach_factory(std::make_shared<ccMongooseWebServerObjectFactory>());

#ifdef	WIN32
    ccWebServerManager::instance().create_web_server("WebServer #1", "8000", "C:\\web_root\\Chatting\\");
#else
    ccWebServerManager::instance().create_web_server("WebServer #1", "8000", ".");
#endif

    ccWebServerManager::instance().add_restful_api(restful_api_);
    ccWebServerManager::instance().add_websocket_manager(websocket_manager_);

    ccWebServerManager::instance().start();
}

ccIoTDeviceManager::~ccIoTDeviceManager() {
    // TODO Auto-generated destructor stub
}

void ccIoTDeviceManager::show() {
    std::lock_guard<std::mutex> lock(mtx_);

    int nIndex = 0;
    for (auto it : agent_map_) {
        std::cout << "-----------------------------------------------" << std::endl;
        std::cout << "* Index: " << (nIndex++ + 1) << std::endl;

        it.second->show();

        std::cout << "-----------------------------------------------" << std::endl;
    }
}

bool ccIoTDeviceManager::sendControlCommand(int index, const std::string command) {
    std::lock_guard<std::mutex> lock(mtx_);

    ccIoTDeviceProtocol control_protocol;

    Json::Value ext_info;

    ext_info["DeviceType"] = "Light";
    ext_info["Control"] = command;

    int count = 0;

    for (auto it : agent_map_) {
        if (count == index) {
            control_protocol.send(it.second->getWebsocket().get(), true, "SetDevice", ext_info);
            return true;
        }

        count++;
    }

    return false;
}

void ccIoTDeviceManager::control_all_switches(bool on) {
    std::lock_guard<std::mutex> lock(mtx_);

    ccIoTDeviceProtocol control_protocol;

    Json::Value ext_info;

    ext_info["DeviceType"] = "Switch";

    if (on)
        ext_info["Control"] = "On";
    else
        ext_info["Control"] = "Off";

    for (auto it : agent_map_) {
        if (it.second->has_device(ccIoTDeviceSpecification::kDeviceType_Switch))
            control_protocol.send(it.second->getWebsocket().get(), true, "SetDevice", ext_info);
    }
}

void ccIoTDeviceManager::control_all_lights(bool on) {
    std::lock_guard<std::mutex> lock(mtx_);

    ccIoTDeviceProtocol control_protocol;

    Json::Value ext_info;

    ext_info["DeviceType"] = "Light";

    if (on)
        ext_info["Control"] = "On";
    else
        ext_info["Control"] = "Off";

    for (auto it : agent_map_) {
        if (it.second->has_device(ccIoTDeviceSpecification::kDeviceType_Light))
            control_protocol.send(it.second->getWebsocket().get(), true, "SetDevice", ext_info);
    }
}

void ccIoTDeviceManager::control_all_locks(bool open) {
    std::lock_guard<std::mutex> lock(mtx_);

    ccIoTDeviceProtocol control_protocol;

    Json::Value ext_info;

    ext_info["DeviceType"] = "Lock";

    if (open)
        ext_info["Control"] = "Open";
    else
        ext_info["Control"] = "Close";

    for (auto it : agent_map_) {
        if (it.second->has_device(ccIoTDeviceSpecification::kDeviceType_Lock))
            control_protocol.send(it.second->getWebsocket().get(), true, "SetDevice", ext_info);
    }
}

//  Open API
bool ccIoTDeviceManager::devices(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response) {
    //std::cout << "enter ccRESTfulChatting::list, query_string=" << request->getQueryString() << endl;

    switch (request->get_method()) {
    case ccWebServerRequest::HttpMethod_Post:   //  Create
        response->send_status(406, std::string("Not Acceptable"));
        response->send_content("");
        break;

    case ccWebServerRequest::HttpMethod_Get:    //  Read
    {
        response->send_status(200, std::string("OK"));

        Json::Value response_json_data;
        Json::Value device_list;
        Json::StyledWriter json_writer;

        device_list["count"] = (int)agent_map_.size();

        int nIndex = 0;

        for (auto it : agent_map_) {
            Json::Value oDeviceInfo;

            oDeviceInfo = it.second->get_device_specInfo().to_json();
            oDeviceInfo["ID"] = it.second->get_id();

            device_list["device"][nIndex++] = oDeviceInfo;
        }

        response_json_data["device_list"] = device_list;

        std::string strJsonData = json_writer.write(response_json_data);

        response->send_content_type("application/javascript", strJsonData.length());
        response->send_content(strJsonData);

        return true;
    }
    break;

    case ccWebServerRequest::HttpMethod_Delete:   //  Delete
        response->send_status(406, std::string("Not Acceptable"));
        response->send_content("");
        break;

    default:
        break;
    }

    return false;
}

bool ccIoTDeviceManager::devices_device(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response) {
    //std::cout << "enter ccRESTfulChatting::list, query_string=" << request->getQueryString() << endl;

    switch (request->get_method()) {
    case ccWebServerRequest::HttpMethod_Post:   //  Create
        response->send_status(406, std::string("Not Acceptable"));
        response->send_content("");
        break;

    case ccWebServerRequest::HttpMethod_Get:    //  Read
    {
        std::string strID = request->get_variable("ID");

        int device_id = std::stoi(strID);

        auto it = agent_map_.find(device_id);

        if (it == std::end(agent_map_)) {
            response->send_status(404, std::string("Not Found"));
            return false;
        }

        response->send_status(200, std::string("OK"));

        Json::Value response_json_data;
        Json::StyledWriter json_writer;

        Json::Value oDeviceInfo;

        oDeviceInfo = it->second->get_device_specInfo().to_json();
        oDeviceInfo["ID"] = it->second->get_id();

        response_json_data["device"] = oDeviceInfo;

        std::string strJsonData = json_writer.write(response_json_data);

        response->send_content_type("application/javascript", strJsonData.length());
        response->send_content(strJsonData);

        return true;
    }
    break;

    case ccWebServerRequest::HttpMethod_Put:    //  Control
    {
        int device_id = std::stoi(request->get_variable("ID"));

        auto it = agent_map_.find(device_id);

        if (it == std::end(agent_map_)) {
            response->send_status(404, std::string("Not Found"));
            return false;
        }

        response->send_status(200, std::string("OK"));

        std::string strControlData;

        request->get_content_body(strControlData);

        ccIoTDeviceProtocol control_protocol;
        control_protocol.parse(strControlData);
        control_protocol.send(it->second->getWebsocket().get());

        response->send_content("");
        return true;
    }
    break;

    case ccWebServerRequest::HttpMethod_Delete:   //  Delete
        response->send_status(406, std::string("Not Acceptable"));
        response->send_content("");
        break;

    default:
        break;
    }

    return false;
}


//  WebSocket
bool ccIoTDeviceManager::ws_iot_device(ccWebsocket::ccWebSocketEvent ws_event, std::shared_ptr<ccWebsocket> websocket, const std::string data) {
    switch ((int)ws_event) {
    case ccWebsocket::ccWebSocketEvent_Connected:
        //  std::cout << "ccIoTDeviceManager: An IoT device is connected. { " << std::endl << data << std::endl << "}" << std::endl;
        break;

    case ccWebsocket::ccWebSocketEvent_ReceivedData:
        //  std::cout << "ccIoTDeviceManager: We got the ommmand from IoT Device. { " << std::endl << data << std::endl << "}" << std::endl;
        parse_command(websocket, data);
        break;

    case ccWebsocket::ccWebSocketEvent_Disconnected:
        //  std::cout << "ccIoTDeviceManager: An IoT device is disconnected." << std::endl;
    {
        ccIoTDeviceProtocol oNoProtocol;

        oNoProtocol.is_request_ = true;

        deregister_command(websocket, oNoProtocol);
    }
    break;
    }

    return true;
}

bool ccIoTDeviceManager::find_device_agent(std::shared_ptr<ccWebsocket> websocket, std::shared_ptr<ccIoTDeviceAgent>& device_agent) {
    auto it = agent_map_.find(websocket->get_instance());

    if (it == std::end(agent_map_))
        return false;

    device_agent = it->second;

    return true;
}

bool  ccIoTDeviceManager::parse_command(std::shared_ptr<ccWebsocket> websocket, const std::string data) {
    ccIoTDeviceProtocol protocol;
    if (protocol.parse(data)) {
        auto it = command_map_.find(protocol.command_);

        if (it != std::end(command_map_))
            it->second(websocket, protocol);
    }

    return true;
}


bool ccIoTDeviceManager::register_command(std::shared_ptr<ccWebsocket> websocket, ccIoTDeviceProtocol& protocol) {
    if (protocol.is_request_ == false)
        return false;

    std::shared_ptr<ccIoTDeviceAgent> pNewDeviceAgent(new ccIoTDeviceAgent(websocket, protocol.specification_list_list_));

    std::lock_guard<std::mutex> lock(mtx_);

    auto it = agent_map_.find(websocket->get_instance());

    if (it != std::end(agent_map_))
        return false;

    agent_map_[websocket->get_instance()] = pNewDeviceAgent;

    //  std::cout << "Registerd: " << pNewDeviceAgent->get_type_name() << ", " << pNewDeviceAgent->get_name() << std::endl;

    return true;
}

bool ccIoTDeviceManager::deregister_command(std::shared_ptr<ccWebsocket> websocket, ccIoTDeviceProtocol& protocol) {
    if (protocol.is_request_ == false)
        return false;

    std::lock_guard<std::mutex> lock(mtx_);

    auto it = agent_map_.find(websocket->get_instance());

    if (it == std::end(agent_map_))
        return false;

    //  std::cout << "DeRegisterd: " << it->second->get_type_name() << ", " << it->second->get_name() << std::endl;

    agent_map_.erase(it);

    return false;
}

bool ccIoTDeviceManager::get_device_status_command(std::shared_ptr<ccWebsocket> websocket, ccIoTDeviceProtocol& protocol) {
    if (protocol.is_request_ == true)
        return false;

    std::lock_guard<std::mutex> lock(mtx_);

    auto it = agent_map_.find(websocket->get_instance());

    if (it == std::end(agent_map_))
        return false;

    return false;
}

bool ccIoTDeviceManager::update_device_status_command(std::shared_ptr<ccWebsocket> websocket, ccIoTDeviceProtocol& protocol) {
    if (protocol.is_request_ == false)
        return false;

    std::lock_guard<std::mutex> lock(mtx_);

    auto it = agent_map_.find(websocket->get_instance());

    if (it == std::end(agent_map_))
        return false;

    return false;
}

bool ccIoTDeviceManager::set_control_command(std::shared_ptr<ccWebsocket> websocket, ccIoTDeviceProtocol& protocol) {
    //  We need to implement this feature.

    return false;
}
