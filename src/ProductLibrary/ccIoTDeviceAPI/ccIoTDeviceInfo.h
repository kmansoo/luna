/*
 * ccIoTDeviceInfo.h
 *
 *  Created on: 2016. 1. 29.
 *      Author: kmansoo
 */

#ifndef PRODUCTLIBRARY_CCIOTDEVICEAPI_CCIOTDEVICEINFO_H_
#define PRODUCTLIBRARY_CCIOTDEVICEAPI_CCIOTDEVICEINFO_H_

#include <string>

#include "ccJsonParserAPI/json/value.h"
#include "ccIoTDeviceProtocolAPI/ccIoTDeviceSpecificationList.h"

class ccIoTDeviceInfo
{
public:
    ccIoTDeviceInfo();
    ccIoTDeviceInfo(const ccIoTDeviceInfo& other);
    ccIoTDeviceInfo(ccIoTDeviceInfo&& other);

    virtual ~ccIoTDeviceInfo();

public:
    ccIoTDeviceInfo&    operator=(ccIoTDeviceInfo&& other);

    bool    LoadFile(const std::string& strFileName);

    bool    Parse(const Json::Value& oInfo);
    bool    Parse(const std::string& strInfo);

public:
    const std::string&            GetMasterUri() { return _strIoTDeviceMasterUri; }

    ccIoTDeviceSpecificationList& GetSpecificationInfo() { return _aSpecificationInfo; }

    int                           GetSpecificationInfoSize() { return _aSpecificationInfo.Size(); }
    ccIoTDeviceSpecification&     GetDeviceSpecification(int nIndex) { return _aSpecificationInfo.GetAt(nIndex); }

private:
    ccIoTDeviceSpecificationList  _aSpecificationInfo;            //  Device Specfication
    std::string                   _strIoTDeviceMasterUri;
};

#endif /* PRODUCTLIBRARY_CCIOTDEVICEPROTOCOLAPI_CCIOTDEVICESPECIFICATION_H_ */
