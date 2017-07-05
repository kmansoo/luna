/*
 * ccIoTDevice.cpp
 *
 *  Created on: 2016. 1. 26.
 *      Author: kmansoo
 */

#include <iostream>

#include "ccCore/ccTimer.h"

#include "ccIoTDevice/ccIoTDevice.h"

using namespace Luna;

ccIoTDevice::ccIoTDevice(const std::string& strSpecFile) : is_connected_(false), is_stop_by_user_(false) {
    command_map_["SetDevice"] = std::bind(&ccIoTDevice::set_device_command, this, std::placeholders::_1);
    command_map_["GetDeviceStatus"] = std::bind(&ccIoTDevice::get_device_status_command, this, std::placeholders::_1);

    ws_client_.set_event_listener(std::bind(&ccIoTDevice::recv_data_from_websocket, this, std::placeholders::_1, std::placeholders::_2));

    if (my_device_info_.load_file(strSpecFile) == false) {
        std::cout << "[Warning!!]" << std::endl;
        std::cout << "  --> Couldn't load the device specification['" << strSpecFile << "'" << std::endl;
        std::cout << "  --> You have to define this device information using '" << strSpecFile << "'" << std::endl;
        std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    }
}

ccIoTDevice::~ccIoTDevice() {
    stop();
}

bool ccIoTDevice::attach_factory(std::shared_ptr<ccIoTDeviceTransportFactory> factory) {

    return false;
}

bool ccIoTDevice::start() {
    if (is_connected_)
        return true;

    is_stop_by_user_ = false;

    target_uri_ = my_device_info_.get_master_uri();

    if (target_uri_ == "")
        target_uri_ = "ws://localhost:8000/ws_iot_deivce";

    retry_connect();

    return true;
}

bool ccIoTDevice::stop() {
    is_stop_by_user_ = true;

    if (!is_connected_)
        return true;

    ccIoTDeviceProtocol protocol;
    protocol.send(&ws_client_, true, "DeRegister");

    //  ccTimer oRetryTimer(500, true, std::bind(&ccIoTDevice::DoCloseWS, this));

    is_connected_ = false;

    return ws_client_.close();
}

bool ccIoTDevice::send(ccIoTDeviceProtocol& protocol) {
    return protocol.send(&ws_client_);
}

bool ccIoTDevice::has_device(ccIoTDeviceSpecification::IoTDeviceType device_type) {
    return my_device_info_.get_specification_info().has_device(device_type);
}

bool ccIoTDevice::has_device(const std::string& device_name) {
    return my_device_info_.get_specification_info().has_device(device_name);
}

//  virtual functions
bool ccIoTDevice::on_recv_command(ccIoTDeviceProtocol& protocol) {
    std::cout << "ccIoTDevice: OnRecvCommand, " << std::endl;

    auto it = command_map_.find(protocol.command_);

    if (it != std::end(command_map_))
        it->second(protocol);

    return true;
}

void ccIoTDevice::retry_connect() {
    if (is_stop_by_user_)
        return;

    std::cout << "ccIoTDevice: retryConnect, " << std::endl;

    ws_client_.close();

    if (ws_client_.open(target_uri_) == false) {
        ccTimer oRetryTimer(5000, true, std::bind(&ccIoTDevice::retry_connect, this));
    }

    is_connected_ = true;

    ccIoTDeviceProtocol protocol;
    protocol.send(&ws_client_, true, "Register", my_device_info_.get_specification_info().to_json());
}

//void ccIoTDevice::DoCloseWS()
//{
//
//}

//  
void ccIoTDevice::recv_data_from_websocket(ccWebsocket::ccWebSocketEvent ws_event, const std::string& message) {
    switch ((int)ws_event) {
    case ccWebsocket::ccWebSocketEvent_Connected:
    {
        is_connected_ = true;
        ccIoTDeviceProtocol protocol;
        protocol.send(&ws_client_, true, "Register", my_device_info_.get_specification_info().to_json());

        std::cout << "ccIoTDevice: Connected" << std::endl;
    }
    break;

    case ccWebsocket::ccWebSocketEvent_Disconnected:
    {
        is_connected_ = false;

        std::cout << "ccIoTDevice: Disconnected" << std::endl;

        if (!is_stop_by_user_)
            ccTimer oRetryTimer(5000, true, std::bind(&ccIoTDevice::retry_connect, this));
    }
    break;

    case ccWebsocket::ccWebSocketEvent_ReceivedData:
    {
        std::cout << "ccIoTDevice: ReceivedData, " << message << std::endl;

        ccIoTDeviceProtocol protocol;

        if (protocol.parse(message))
            on_recv_command(protocol);
    }
    break;
    }
}

bool ccIoTDevice::set_device_command(ccIoTDeviceProtocol& protocol) {
    if (protocol.is_request_ == false)
        return false;

    //std::cout << "ccIoTDevice: setDeviceCommand " << std::endl;
    //ccIoTDeviceProtocol oResponseProtocol;
    //oResponseProtocol.send(&ws_client_, false, protocol.command_);

    return true;
}

bool ccIoTDevice::get_device_status_command(ccIoTDeviceProtocol& protocol) {
    if (protocol.is_request_ == false)
        return false;

    //std::cout << "ccIoTDevice: getDeviceStatusCommand " << std::endl;
    //ccIoTDeviceProtocol oResponseProtocol;
    //oResponseProtocol.send(&ws_client_, false, protocol.command_);

    return true;
}
