/*
 * ccIoTDeviceSpecificationList.cpp
 *
 *  Created on: 2016. 1. 29.
 *      Author: kmansoo
 */

#include <fstream>
#include <string.h>

#include "json/reader.h"

#include "ccIoTDevice/ccIoTDeviceSpecificationList.h"

ccIoTDeviceSpecificationList::ccIoTDeviceSpecificationList() {
}

ccIoTDeviceSpecificationList::ccIoTDeviceSpecificationList(const ccIoTDeviceSpecificationList& other) {
    this->specification_json_ = other.specification_json_;
    //this->specification_list_.resize(other.specification_list_.size());

    memcpy(&this->specification_list_.at(0), &other.specification_list_.at(0), other.specification_list_.size());
}


ccIoTDeviceSpecificationList::ccIoTDeviceSpecificationList(ccIoTDeviceSpecificationList&& other) {
    *this = std::move(other);
}

ccIoTDeviceSpecificationList::~ccIoTDeviceSpecificationList() {
    // TODO Auto-generated destructor stub
}

ccIoTDeviceSpecificationList& ccIoTDeviceSpecificationList::operator=(ccIoTDeviceSpecificationList&& other) {
    if (this == &other)
        return *this;

    this->specification_json_ = std::move(other.specification_json_);
    this->specification_list_ = std::move(other.specification_list_);

    return *this;
}

bool ccIoTDeviceSpecificationList::loca_file(const std::string& file_name) {
    std::ifstream ifs(file_name);

    if (!ifs)
        return false;

    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    return parse(content);
}

bool ccIoTDeviceSpecificationList::parse(const Json::Value& info) {
    if (info.isNull())
        return false;

    specification_json_["IoTDeviceSpecification"] = info;

    if (info.isArray() == false) {
        ccIoTDeviceSpecification    device_spec;

        if (device_spec.parse(info))
            specification_list_.push_back(std::move(device_spec));
    } else {
        for (Json::ArrayIndex index = 0; index < info.size(); index++) {
            ccIoTDeviceSpecification    device_spec;

            if (device_spec.parse(info[index]) == true)
                specification_list_.push_back(std::move(device_spec));
        }
    }

    return true;
}

bool ccIoTDeviceSpecificationList::parse(const std::string& info) {
    Json::Reader    json_reader;
    Json::Value     root_value;

    if (!json_reader.parse(info, root_value))
        return false;

    return parse(root_value);
}

//  public method
std::size_t ccIoTDeviceSpecificationList::size() {
    return specification_list_.size();
}

std::vector<ccIoTDeviceSpecification>&  ccIoTDeviceSpecificationList::get_list() {
    return specification_list_;
}

bool ccIoTDeviceSpecificationList::has_device(ccIoTDeviceSpecification::IoTDeviceType device_type) {
    for (auto it : specification_list_) {
        if (it.get_type() == device_type)
            return true;
    }

    return false;
}

bool ccIoTDeviceSpecificationList::has_device(const std::string& device_name) {
    return has_device(ccIoTDeviceSpecification::getType(device_name));
}

int ccIoTDeviceSpecificationList::get_device_index(ccIoTDeviceSpecification::IoTDeviceType device_type) {
    for (std::size_t index = 0; index < specification_list_.size(); index++) {
        if (specification_list_[index].get_type() == device_type)
            return index;
    }

    return -1;
}

ccIoTDeviceSpecification& ccIoTDeviceSpecificationList::get_at(std::size_t index) {
    if (index >= specification_list_.size())
        return blank_string_data_;

    return specification_list_[index];
}

const Json::Value& ccIoTDeviceSpecificationList::to_json() {
    return specification_json_;
}
