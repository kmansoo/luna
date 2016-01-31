/*
 * ccIoTDeviceSpecification.h
 *
 *  Created on: 2016. 1. 29.
 *      Author: kmansoo
 */

#ifndef PRODUCTLIBRARY_CCIOTDEVICEPROTOCOLAPI_CCIOTDEVICESPECIFICATION_H_
#define PRODUCTLIBRARY_CCIOTDEVICEPROTOCOLAPI_CCIOTDEVICESPECIFICATION_H_

#include <string>

#include "ccJsonParserAPI/json/value.h"

class ccIoTDeviceSpecification
{
public:
    ccIoTDeviceSpecification();
    virtual ~ccIoTDeviceSpecification();

public:
    bool    LoadFile(const std::string& strFileName);

    bool    Parse(const Json::Value& oInfo);
    bool    Parse(const std::string& strInfo);

public:
    enum IoTDeviceType {
        kDeviceType_Unknown,
        kDeviceType_Switch,
        kDeviceType_Light,
        kDeviceType_Lock,
    };

public:
    const std::string& GetTypeName();
    const IoTDeviceType GetType() { return _eDeviceType; }
    const std::string& GetName() { return _strName; }
    const std::string& GetDescription() { return _strDescription; }
    const std::string& GetManufacture() { return _strManufacture; }

    const Json::Value& ToJson() { return _oSpecificationValue; }

protected:
    //  Example
    //
    //  {
    //      "IoTDeviceSpecification" : {
    //          "Type" : "Switch",
    //          "Name" : "Simple Switch",
    //          "Description" : "On/Off",
    //          "Manufacture" : "Mansoo"
    //      }
    //  }
    //
    Json::Value     _oSpecificationValue;

    IoTDeviceType   _eDeviceType;
    std::string     _strName;
    std::string     _strDescription;
    std::string     _strManufacture;
};

#endif /* PRODUCTLIBRARY_CCIOTDEVICEPROTOCOLAPI_CCIOTDEVICESPECIFICATION_H_ */
