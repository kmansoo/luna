/*
 * ccIoTDeviceSpecificationList.h
 *
 *  Created on: 2016. 1. 29.
 *      Author: kmansoo
 */

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "json/value.h"

#include "ccIoTDeviceSpecification.h"

class ccIoTDeviceSpecificationList {
public:
    ccIoTDeviceSpecificationList();
    ccIoTDeviceSpecificationList(const ccIoTDeviceSpecificationList& other);

    virtual ~ccIoTDeviceSpecificationList();

public:
    bool    load_file(const std::string& file_name);

    bool    parse(const Json::Value& info);
    bool    parse(const std::string& info);

public:
    ccIoTDeviceSpecificationList& operator=(ccIoTDeviceSpecificationList& other);

    std::size_t size();

    std::vector<std::shared_ptr<ccIoTDeviceSpecification>>& get_list();
    ccIoTDeviceSpecification& get_at(std::size_t index);
    
    const Json::Value& to_json();

    bool has_device(ccIoTDeviceSpecification::IoTDeviceType device_type);
    bool has_device(const std::string& device_name);

    int  get_device_Index(ccIoTDeviceSpecification::IoTDeviceType device_type);

protected:
    Json::Value specification_json_;

    ccIoTDeviceSpecification                blank_string_data_;
    std::vector<std::shared_ptr<ccIoTDeviceSpecification>>   specification_list_;            //  Device Specfication

};
