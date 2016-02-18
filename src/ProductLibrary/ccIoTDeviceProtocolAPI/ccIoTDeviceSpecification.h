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
    ccIoTDeviceSpecification(const ccIoTDeviceSpecification& other);
    ccIoTDeviceSpecification(ccIoTDeviceSpecification&& other);

    virtual ~ccIoTDeviceSpecification();

public:
    bool    Parse(const Json::Value& oInfo);
    bool    Parse(const std::string& strInfo);

public:
    enum IoTDeviceType {
        kDeviceType_Unknown,
        kDeviceType_Switch,
        kDeviceType_Light,
        kDeviceType_Lock,
        kDeviceType_Sensor,
        kDeviceType_Controller,

        kDeviceType_Max,
    };

public:
    static const std::string&   getTypeNameString(IoTDeviceType eType);
    static IoTDeviceType        getType(const std::string& strName);

public:
    ccIoTDeviceSpecification& operator=(ccIoTDeviceSpecification&& other);

    const IoTDeviceType GetType() { return _eDeviceType; }
    const std::string& GetTypeName();
    const std::string& GetName() { return _strName; }
    const std::string& GetDescription() { return _strDescription; }
    const std::string& GetManufacture() { return _strManufacture; }

protected:
    //  Example
    //
    //  -- Single Device
    //  {
    //      "IoTDeviceSpecification" : {
    //          "Type" : "Switch",
    //          "Name" : "Simple Switch",
    //          "Description" : "On/Off",
    //          "Manufacture" : "Mansoo"
    //      }
    //  }
    //
    //  -- Multi Device
    //  {
    //      "IoTDeviceSpecification" : [
    //          {
    //              "Type" : "Light"
    //              "Name" : "Linux Virtual Smart Light",
    //              "Description" : "On/Off",
    //              "Manufacture" : "Mansoo",
    //          },
    //          {
    //              "Type" : "Switch",
    //              "Name" : "Linux Virtual Smart Switch",
    //              "Description" : "On/Off",
    //              "Manufacture" : "Mansoo"
    //          }
    //      ]
    //  }
    //
    IoTDeviceType   _eDeviceType;
    std::string     _strName;
    std::string     _strDescription;
    std::string     _strManufacture;
};

#endif /* PRODUCTLIBRARY_CCIOTDEVICEPROTOCOLAPI_CCIOTDEVICESPECIFICATION_H_ */
