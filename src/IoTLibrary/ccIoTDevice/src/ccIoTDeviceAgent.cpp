/*
 * ccIoTDeviceAgent.cpp
 *
 *  Created on: 2016. 1. 28.
 *      Author: kmansoo
 */

#include <iostream>

#include "ccIoTDevice/ccIoTDeviceAgent.h"

using namespace Luna;

ccIoTDeviceAgent::ccIoTDeviceAgent(std::shared_ptr<ccWebsocket> websocket, ccIoTDeviceSpecificationList& spec_info) {
    websocket_ = websocket;

    device_spec_info_ = spec_info;
}

ccIoTDeviceAgent::~ccIoTDeviceAgent() {
    // TODO Auto-generated destructor stub
}

bool ccIoTDeviceAgent::has_device(ccIoTDeviceSpecification::IoTDeviceType device_type) {
    return device_spec_info_.has_device(device_type);
}

bool ccIoTDeviceAgent::has_device(const std::string& device_name) {
    return device_spec_info_.has_device(device_name);
}

int ccIoTDeviceAgent::get_device_Index(ccIoTDeviceSpecification::IoTDeviceType device_type) {
    return device_spec_info_.get_device_Index(device_type);
}

ccIoTDeviceSpecification& ccIoTDeviceAgent::get_device_spec(std::size_t nIndex) {
    if (nIndex >= device_spec_info_.size())
        return null_obj_;

    return device_spec_info_.get_at(nIndex);
}

ccIoTDeviceSpecification::IoTDeviceType ccIoTDeviceAgent::get_type(std::size_t nIndex) {
    if (nIndex >= device_spec_info_.size())
        return ccIoTDeviceSpecification::kDeviceType_Unknown;

    return device_spec_info_.get_at(nIndex).get_type();
}

const std::string& ccIoTDeviceAgent::get_type_name(std::size_t nIndex) {
    if (nIndex >= device_spec_info_.size())
        return blank_string_data_;

    return device_spec_info_.get_at(nIndex).get_type_name();
}

const std::string& ccIoTDeviceAgent::get_name(std::size_t nIndex) {
    if (nIndex >= device_spec_info_.size())
        return blank_string_data_;

    return device_spec_info_.get_at(nIndex).get_name();
}

void ccIoTDeviceAgent::show() {
    std::cout << "* ID               : " << websocket_->get_instance() << std::endl;

    for (std::size_t nIndex = 0; nIndex < device_spec_info_.size(); nIndex++) {
        std::cout << "  ** Functon       : " << nIndex + 1 << std::endl;
        std::cout << "       Type        : " << device_spec_info_.get_at(nIndex).get_type_name() << std::endl;
        std::cout << "       Name        : " << device_spec_info_.get_at(nIndex).get_name() << std::endl;
        std::cout << "       Description : " << device_spec_info_.get_at(nIndex).get_description() << std::endl;
        std::cout << "       Manufacture : " << device_spec_info_.get_at(nIndex).get_manufacture() << std::endl;
    }
}
