/*
 * ccIoTDeviceSpecificationList.h
 *
 *  Created on: 2016. 1. 29.
 *      Author: kmansoo
 */

#ifndef PRODUCTLIBRARY_CCIOTDEVICEPROTOCOLAPI_CCIOTDEVICESPECIFICATIONLIST_H_
#define PRODUCTLIBRARY_CCIOTDEVICEPROTOCOLAPI_CCIOTDEVICESPECIFICATIONLIST_H_

#include <string>
#include <vector>

#include "json/value.h"

#include "ccIoTDeviceSpecification.h"

class ccIoTDeviceSpecificationList {
public:
    ccIoTDeviceSpecificationList();
    ccIoTDeviceSpecificationList(const ccIoTDeviceSpecificationList& other);
    ccIoTDeviceSpecificationList(ccIoTDeviceSpecificationList&& other);

    virtual ~ccIoTDeviceSpecificationList();

public:
    bool    loca_file(const std::string& file_name);

    bool    parse(const Json::Value& info);
    bool    parse(const std::string& info);

public:
    ccIoTDeviceSpecificationList& operator=(ccIoTDeviceSpecificationList&& other);

    std::size_t                             size();
    std::vector<ccIoTDeviceSpecification>&  get_list();
    ccIoTDeviceSpecification&               get_at(std::size_t index);
    const Json::Value&                      to_json();

    bool has_device(ccIoTDeviceSpecification::IoTDeviceType device_type);
    bool has_device(const std::string& device_name);

    int  get_device_index(ccIoTDeviceSpecification::IoTDeviceType device_type);

protected:
    Json::Value specification_json_;

    ccIoTDeviceSpecification                blank_string_data_;
    std::vector<ccIoTDeviceSpecification>   specification_list_;            //  Device Specfication

};

#endif /* PRODUCTLIBRARY_CCIOTDEVICEPROTOCOLAPI_ccIoTDeviceSpecificationList_H_ */
