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
    ccIoTDeviceInfo(ccIoTDeviceInfo&& other);

    virtual ~ccIoTDeviceInfo();

public:
    ccIoTDeviceInfo&    operator=(ccIoTDeviceInfo&& other);

    bool    load_file(const std::string& file_name);

    bool    parse(const Json::Value& info);
    bool    parse(const std::string& info);

public:
    const std::string&            get_master_uri() { return iot_device_master_uri_; }

    ccIoTDeviceSpecificationList& get_specification_info() { return specification_info_; }

    int                           get_specification_info_size() { return specification_info_.size(); }
    ccIoTDeviceSpecification&     get_device_specification(int nIndex) { return specification_info_.get_at(nIndex); }

private:
    ccIoTDeviceSpecificationList  specification_info_;            //  Device Specfication
    std::string                   iot_device_master_uri_;
};

#endif /* PRODUCTLIBRARY_CCIOTDEVICEPROTOCOLAPI_CCIOTDEVICESPECIFICATION_H_ */
