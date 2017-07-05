/*
 * ccIoTDeviceSpecification.h
 *
 *  Created on: 2016. 1. 29.
 *      Author: kmansoo
 */

#ifndef PRODUCTLIBRARY_CCIOTDEVICEPROTOCOLAPI_CCIOTDEVICESPECIFICATION_H_
#define PRODUCTLIBRARY_CCIOTDEVICEPROTOCOLAPI_CCIOTDEVICESPECIFICATION_H_

#include <string>

#include "json/value.h"

class ccIoTDeviceSpecification {
public:
    ccIoTDeviceSpecification();
    ccIoTDeviceSpecification(const ccIoTDeviceSpecification& other);

    virtual ~ccIoTDeviceSpecification();

public:
    bool    parse(const Json::Value& info);
    bool    parse(const std::string& info);

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
    static const std::string&   get_type_name_string(IoTDeviceType device_type);
    static IoTDeviceType        get_type(const std::string& name);

public:
    ccIoTDeviceSpecification& operator=(ccIoTDeviceSpecification& other);

    const Json::Value&  get_specification() { return specfication_; }

    const IoTDeviceType get_type() { return device_type_; }
    const std::string& get_type_name();
    const std::string& get_name() { return name_; }
    const std::string& get_description() { return description_; }
    const std::string& get_manufacture() { return manufacture_; }

    const std::string get_item_text(const std::string& key);

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
    Json::Value     specfication_;

    IoTDeviceType   device_type_;
    std::string     name_;
    std::string     description_;
    std::string     manufacture_;
};

#endif /* PRODUCTLIBRARY_CCIOTDEVICEPROTOCOLAPI_CCIOTDEVICESPECIFICATION_H_ */
