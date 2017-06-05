/*
 * ccIoTDeviceInfo.cpp
 *
 *  Created on: 2016. 1. 29.
 *      Author: kmansoo
 */

#include <fstream>

#include "json/reader.h"

#include "ccIoTDevice/ccIoTDeviceInfo.h"

ccIoTDeviceInfo::ccIoTDeviceInfo() {
}

ccIoTDeviceInfo::ccIoTDeviceInfo(const ccIoTDeviceInfo& other) :
    specification_info_(other.specification_info_),
    iot_device_master_uri_(other.iot_device_master_uri_) {
}

ccIoTDeviceInfo::ccIoTDeviceInfo(ccIoTDeviceInfo&& other) {
    *this = std::move(other);
}

ccIoTDeviceInfo::~ccIoTDeviceInfo() {
    // TODO Auto-generated destructor stub
}

ccIoTDeviceInfo& ccIoTDeviceInfo::operator=(ccIoTDeviceInfo&& other) {
    if (this == &other)
        return *this;

    specification_info_ = std::move(other.specification_info_);
    iot_device_master_uri_ = std::move(other.iot_device_master_uri_);

    return *this;
}


bool ccIoTDeviceInfo::load_file(const std::string& file_name) {
    std::ifstream ifs(file_name);

    if (!ifs)
        return false;

    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    return parse(content);
}

bool ccIoTDeviceInfo::parse(const Json::Value& info) {
    if (info.isObject() == false)
        return false;

    if (info["IoTDeviceSpecification"].isNull())
        return false;

    if (info["IoTDeviceMasterUri"].isNull())
        return false;

    specification_info_.parse(info["IoTDeviceSpecification"]);
    iot_device_master_uri_ = info["IoTDeviceMasterUri"].asString();

    return true;
}

bool ccIoTDeviceInfo::parse(const std::string& info) {
    Json::Reader    json_reader;
    Json::Value     root_value;

    if (!json_reader.parse(info, root_value))
        return false;

    return parse(root_value);
}
