/*
 * ccIoTDeviceSpecification.cpp
 *
 *  Created on: 2016. 1. 29.
 *      Author: kmansoo
 */

#include <fstream>

#include "json/reader.h"

#include "ccIoTDevice/ccIoTDeviceSpecification.h"

std::string s_device_type_name_list[]{
    "Unknown",
    "Switch",
    "Light",
    "Lock",
    "Sensor",
    "Controller"
};

ccIoTDeviceSpecification::ccIoTDeviceSpecification() {
    device_type_ = kDeviceType_Unknown;
}

ccIoTDeviceSpecification::ccIoTDeviceSpecification(const ccIoTDeviceSpecification& other) {
    device_type_ = other.device_type_;
    name_ = other.name_;
    description_ = other.description_;
}


ccIoTDeviceSpecification::ccIoTDeviceSpecification(ccIoTDeviceSpecification&& other) {
    *this = std::move(other);
}

ccIoTDeviceSpecification::~ccIoTDeviceSpecification() {
    // TODO Auto-generated destructor stub
}

const std::string& ccIoTDeviceSpecification::getTypeNameString(IoTDeviceType device_type) {
    if ((std::uint16_t)device_type >= kDeviceType_Max)
        return s_device_type_name_list[0];

    return s_device_type_name_list[device_type];
}

ccIoTDeviceSpecification::IoTDeviceType ccIoTDeviceSpecification::getType(const std::string& name) {
    for (int index = 0; index < kDeviceType_Max; index++) {
        if (name == s_device_type_name_list[index])
            return (IoTDeviceType)index;
    }

    return kDeviceType_Unknown;
}

ccIoTDeviceSpecification& ccIoTDeviceSpecification::operator=(ccIoTDeviceSpecification&& other) {
    if (this == &other)
        return *this;

    device_type_ = other.device_type_;
    name_ = std::move(other.name_);
    description_ = std::move(other.description_);

    return *this;
}

const std::string& ccIoTDeviceSpecification::get_type_name() {
    return getTypeNameString(device_type_);
}

bool ccIoTDeviceSpecification::parse(const Json::Value& info) {
    if (info.isObject() == false)
        return false;

    if (info["Type"].isNull())
        return false;

    if (info["Name"].isNull())
        return false;

    device_type_ = getType(info["Type"].asString());
    name_ = info["Name"].asCString();

    if (!info["Description"].isNull())
        description_ = info["Description"].asString();

    if (!info["Manufacture"].isNull())
        manufacture_ = info["Manufacture"].asString();

    return true;
}

bool ccIoTDeviceSpecification::parse(const std::string& info) {
    Json::Reader    json_reader;
    Json::Value     root_value;

    if (!json_reader.parse(info, root_value))
        return false;

    return parse(root_value);
}
