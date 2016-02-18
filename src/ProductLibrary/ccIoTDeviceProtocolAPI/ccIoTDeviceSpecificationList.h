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

#include "ccJsonParserAPI/json/value.h"

#include "ccIoTDeviceSpecification.h"

class ccIoTDeviceSpecificationList
{
public:
    ccIoTDeviceSpecificationList();
    ccIoTDeviceSpecificationList(const ccIoTDeviceSpecificationList& other);
    ccIoTDeviceSpecificationList(ccIoTDeviceSpecificationList&& other);

    virtual ~ccIoTDeviceSpecificationList();

public:
    bool    LoadFile(const std::string& strFileName);

    bool    Parse(const Json::Value& oInfo);
    bool    Parse(const std::string& strInfo);

public:
    ccIoTDeviceSpecificationList& operator=(ccIoTDeviceSpecificationList&& other);

    std::size_t                             Size();
    std::vector<ccIoTDeviceSpecification>&  GetList();
    ccIoTDeviceSpecification&               GetAt(std::size_t nIndex);
    const Json::Value&                      ToJson();

    bool HasDevice(ccIoTDeviceSpecification::IoTDeviceType eDeviceType);
    bool HasDevice(const std::string& strDeviceName);

    int  GetDeviceIndex(ccIoTDeviceSpecification::IoTDeviceType eDeviceType);

protected:
    Json::Value _oSpecificationValue;

    ccIoTDeviceSpecification                _oNullObj;
    std::vector<ccIoTDeviceSpecification>   _aSpecificationList;            //  Device Specfication

};

#endif /* PRODUCTLIBRARY_CCIOTDEVICEPROTOCOLAPI_ccIoTDeviceSpecificationList_H_ */
