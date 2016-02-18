/*
 * ccIoTDeviceProtocol.cpp
 *
 *  Created on: 2016. 1. 29.
 *      Author: kmansoo
 */

#include <iostream>

#include "ccJsonParserAPI/json/value.h"
#include "ccJsonParserAPI/json/writer.h"
#include "ccJsonParserAPI/json/reader.h"

#include "ccIoTDeviceProtocol.h"

//  Protocol Example
//  "Register" : Single Device
//{
//    "Request" : true,
//    "Command" : "Register",
//    "Info" : {
//      "IoTDeviceSpecification" : {
//          "Type" : "Light"
//          "Name" : "Linux Smart Light",
//          "Description" : "On/Off",
//          "Manufacture" : "Mansoo",
//      },
//      "IoTDeviceMasterUri" : "ws://localhost:8000/ws_iot_deivce",
//    }
//}
//
//  "Register"  : Multi Device
//{
//    "Request" : true,
//    "Command" : "Register",
//    "Info" : {
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
//      ],
//      "IoTDeviceMasterUri" : "ws://localhost:8000/ws_iot_deivce",
//    }
//}

//
//
//  "SetDevice"
//{
//    "Request" : true,
//    "Command" : "SetDevice",
//    "Info" : {
//          "DeviceType"    : "Light"
//          "Control"       : "On"
//     },
//}
//
//
//  "SetDevice"
//{
//    "Request" : true,
//    "Command" : "SetDevice",
//    "Info" : {
//          "DeviceType"    : "Light"
//          "Control"       : "On"
//     },
//}
//
ccIoTDeviceProtocol::ccIoTDeviceProtocol()
{
    // TODO Auto-generated constructor stub

    _aParsers["Register"]           = std::bind(&ccIoTDeviceProtocol::DoParserRegisterCommand, this, std::placeholders::_1);
    _aParsers["DeRegister"]         = std::bind(&ccIoTDeviceProtocol::DoParserDeRegisterCommand, this, std::placeholders::_1);
    _aParsers["SetDevice"]          = std::bind(&ccIoTDeviceProtocol::DoParserSetDeviceCommand, this, std::placeholders::_1);
    _aParsers["GetDeviceStatus"]    = std::bind(&ccIoTDeviceProtocol::DoParserGetDeviceStatusCommand, this, std::placeholders::_1);
}

ccIoTDeviceProtocol::~ccIoTDeviceProtocol()
{
    // TODO Auto-generated destructor stub
}

bool ccIoTDeviceProtocol::Send(ccWebsocket* oWS)
{
    return Send(oWS, _IsRequest, _strCommand, _oExtInfo);
}

bool ccIoTDeviceProtocol::Send(ccWebsocket* oWS, bool bIsRequest, const std::string& strCommand)
{
    Json::Value oNoInfo;

    return Send(oWS, bIsRequest, strCommand, oNoInfo);
}

bool ccIoTDeviceProtocol::Send(ccWebsocket* oWS, bool bIsRequest, const std::string& strCommand, const Json::Value& oExtInfo)
{
    std::string strData;

    Json::Value oProtocol;
    Json::StyledWriter oWriter;

    oProtocol["Request"] = bIsRequest;
    oProtocol["Command"] = strCommand;

    if (!oExtInfo.isNull())
        oProtocol["Info"] = oExtInfo;

    strData = std::move(oWriter.write(oProtocol));

    return oWS->Send(strData);
}

bool ccIoTDeviceProtocol::Parse(const std::string& strData)
{
    //  for debug
    //std::cout << std::endl;
    //std::cout << strData << std::endl;
    //std::cout << std::endl << std::endl;
    Json::Reader    oReader;
    Json::Value     oProtocol;

    if (!oReader.parse(strData, oProtocol))
        return false;

    if (oProtocol.isObject() == false)
        return false;

    if (oProtocol["Request"].isNull())
        return false;

    if (oProtocol["Command"].isNull())
        return false;

    _IsRequest  = oProtocol["Request"].asBool();
    _strCommand = std::move(oProtocol["Command"].asString());

    if (oProtocol["Info"].isObject() == true)
        _oExtInfo = std::move(oProtocol["Info"]);

    auto fun = _aParsers[_strCommand];

    if (fun != NULL)
        fun(_oExtInfo);

    return true;
}

//  I'll implement the following method.
bool ccIoTDeviceProtocol::DoParserRegisterCommand(const Json::Value& oExtInfo)
{
    if (oExtInfo.isNull())
        return false;

    if (oExtInfo["IoTDeviceSpecification"].isNull())
        return false;

    return _aSpecificationInfo.Parse(oExtInfo["IoTDeviceSpecification"]);
}

bool ccIoTDeviceProtocol::DoParserDeRegisterCommand(const Json::Value& oExtInfo)
{

    return false;
}

bool ccIoTDeviceProtocol::DoParserSetDeviceCommand(const Json::Value& oExtInfo)
{

    return false;
}

bool ccIoTDeviceProtocol::DoParserGetDeviceStatusCommand(const Json::Value& oExtInfo)
{

    return false;
}
