/*
 * ccIoTDeviceAgent.h
 *
 *  Created on: 2016. 1. 28.
 *      Author: kmansoo
 */

#pragma once

#include <string>
#include <memory>
#include <vector>

#include "ccWebServer/ccWebsocket.h"
#include "json/value.h"

#include "ccIoTDeviceSpecificationList.h"

class ccIoTDeviceAgent {
public:
    ccIoTDeviceAgent(std::shared_ptr<Luna::ccWebsocket> websocket, ccIoTDeviceSpecificationList& spec_info);
    virtual ~ccIoTDeviceAgent();

public:
    void show();

public:
    int get_device_count() { return (int)device_spec_info_.size(); }

    ccIoTDeviceSpecificationList& get_device_specInfo() {
        return device_spec_info_;
    }  //  for All Device Info

    bool has_device(ccIoTDeviceSpecification::IoTDeviceType device_type);
    bool has_device(const std::string& device_name);

    int get_device_Index(ccIoTDeviceSpecification::IoTDeviceType device_type);

    ccIoTDeviceSpecification& get_device_spec( std::size_t nIndex = 0);  //  for Specific Device

    ccIoTDeviceSpecification::IoTDeviceType get_type(std::size_t nIndex = 0);

    const std::string& get_type_name(std::size_t nIndex = 0);
    const std::string& get_name(std::size_t nIndex = 0);

    std::int32_t get_id() { return websocket_->get_instance(); }

    const std::shared_ptr<Luna::ccWebsocket> getWebsocket() {
        return websocket_;
    }

private:
    std::shared_ptr<Luna::ccWebsocket> websocket_;
    ccIoTDeviceSpecificationList device_spec_info_;

    ccIoTDeviceSpecification null_obj_;
    std::string blank_string_data_;
};
