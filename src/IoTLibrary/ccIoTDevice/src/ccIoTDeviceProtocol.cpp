/*
 * ccIoTDeviceProtocol.cpp
 *
 *  Created on: 2016. 1. 29.
 *      Author: kmansoo
 */

#include <iostream>

#include "json/value.h"
#include "json/writer.h"
#include "json/reader.h"

#include "ccIoTDevice/ccIoTDeviceProtocol.h"

using namespace Luna;

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
ccIoTDeviceProtocol::ccIoTDeviceProtocol() {
    // TODO Auto-generated constructor stub

    command_map_["Register"] = std::bind(&ccIoTDeviceProtocol::parser_register_command, this, std::placeholders::_1);
    command_map_["DeRegister"] = std::bind(&ccIoTDeviceProtocol::parser_deregister_command, this, std::placeholders::_1);
    command_map_["SetDevice"] = std::bind(&ccIoTDeviceProtocol::parser_set_device_command, this, std::placeholders::_1);
    command_map_["GetDeviceStatus"] = std::bind(&ccIoTDeviceProtocol::parser_get_device_status_command, this, std::placeholders::_1);
}

ccIoTDeviceProtocol::~ccIoTDeviceProtocol() {
    // TODO Auto-generated destructor stub
}

bool ccIoTDeviceProtocol::send(ccWebsocket* oWS) {
    return send(oWS, is_request_, command_, ext_info_);
}

bool ccIoTDeviceProtocol::send(ccWebsocket* oWS, bool is_request, const std::string& command) {
    Json::Value oNoInfo;

    return send(oWS, is_request, command, oNoInfo);
}

bool ccIoTDeviceProtocol::send(ccWebsocket* oWS, bool is_request, const std::string& command, const Json::Value& ext_info) {
    std::string data;

    Json::Value protocol_json;
    Json::StyledWriter json_writer;

    protocol_json["Request"] = is_request;
    protocol_json["Command"] = command;

    if (!ext_info.isNull())
        protocol_json["Info"] = ext_info;

    data = json_writer.write(protocol_json);

    return oWS->send(data);
}

bool ccIoTDeviceProtocol::parse(const std::string& data) {
    //  for debug
    //std::cout << std::endl;
    //std::cout << data << std::endl;
    //std::cout << std::endl << std::endl;
    Json::Reader    json_reader;
    Json::Value     protocol_json;

    if (!json_reader.parse(data, protocol_json))
        return false;

    if (protocol_json.isObject() == false)
        return false;

    if (protocol_json["Request"].isNull())
        return false;

    if (protocol_json["Command"].isNull())
        return false;

    is_request_ = protocol_json["Request"].asBool();
    command_ = protocol_json["Command"].asString();

    if (protocol_json["Info"].isObject() == true)
        ext_info_ = protocol_json["Info"];

    auto fun = command_map_[command_];

    if (fun != NULL)
        fun(ext_info_);

    return true;
}

//  I'll implement the following method.
bool ccIoTDeviceProtocol::parser_register_command(const Json::Value& ext_info) {
    if (ext_info.isNull())
        return false;

    if (ext_info["IoTDeviceSpecification"].isNull())
        return false;

    return specification_list_list_.parse(ext_info["IoTDeviceSpecification"]);
}

bool ccIoTDeviceProtocol::parser_deregister_command(const Json::Value& ext_info) {

    return false;
}

bool ccIoTDeviceProtocol::parser_set_device_command(const Json::Value& ext_info) {

    return false;
}

bool ccIoTDeviceProtocol::parser_get_device_status_command(const Json::Value& ext_info) {

    return false;
}
