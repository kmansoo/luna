/*
 * ccIoTDeviceInfo.h
 *
 *  Created on: 2016. 1. 29.
 *      Author: kmansoo
 */

#ifndef PRODUCTLIBRARY_CCIOTDEVICEAPI_CCIOTDEVICEINFO_H_
#define PRODUCTLIBRARY_CCIOTDEVICEAPI_CCIOTDEVICEINFO_H_

#include <string>

#include "json/value.h"
#include "ccIoTDeviceSpecificationList.h"

class ccIoTDeviceInfo {
public:
    ccIoTDeviceInfo();
    ccIoTDeviceInfo(const ccIoTDeviceInfo& other);

    virtual ~ccIoTDeviceInfo();

public:
    ccIoTDeviceInfo&    operator=(ccIoTDeviceInfo& other);

    bool    load_file(const std::string& file_name);

    bool    parse(const Json::Value& info);
    bool    parse(const std::string& info);

public:
    const std::string&            get_master_uri() { return iot_device_master_uri_; }

    ccIoTDeviceSpecificationList& get_specification_info() { return specification_list_; }

    int                           get_specification_infoSize() { return (int)specification_list_.size(); }
    ccIoTDeviceSpecification&     get_device_specification(int nIndex) { return specification_list_.get_at(nIndex); }

private:
    Json::Value                   root_value_;
    ccIoTDeviceSpecificationList  specification_list_;            //  Device Specfication
    std::string                   iot_device_master_uri_;
};

#endif /* PRODUCTLIBRARY_CCIOTDEVICEPROTOCOLAPI_CCIOTDEVICESPECIFICATION_H_ */
