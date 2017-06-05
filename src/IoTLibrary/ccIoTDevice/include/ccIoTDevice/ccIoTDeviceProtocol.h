/*
 * ccIoTDeviceProtocol.h
 *
 *  Created on: 2016. 1. 29.
 *      Author: kmansoo
 */

#ifndef PRODUCTLIBRARY_CCIOTDEVICEPROTOCOLAPI_CCIOTDEVICEPROTOCOL_H_
#define PRODUCTLIBRARY_CCIOTDEVICEPROTOCOLAPI_CCIOTDEVICEPROTOCOL_H_

#include <functional>
#include <string>
#include <vector>
#include <map>

#include "json/value.h"

#include "ccWebServer/ccWebsocket.h"

#include "ccIoTDeviceSpecificationList.h"

class ccIoTDeviceProtocol {
public:
    ccIoTDeviceProtocol();
    virtual ~ccIoTDeviceProtocol();

public:
    bool    send(Luna::ccWebsocket* pWS);
    bool    send(Luna::ccWebsocket* pWS, bool is_request, const std::string& command);
    bool    send(Luna::ccWebsocket* pWS, bool is_request, const std::string& command, const Json::Value& ext_info);

    bool    parse(const std::string& data);

protected:
    bool    parser_register_command(const Json::Value& ext_info);
    bool    parser_deregister_command(const Json::Value& ext_info);
    bool    parser_set_device_command(const Json::Value& ext_info);
    bool    parser_get_device_status_command(const Json::Value& ext_info);

public:
    enum IoTDeviceSwitchControl {
        kDeviceSwitchControl_Off,
        kDeviceSwitchControl_On
    };

    enum IoTDeviceLightCommand {
        kDeviceLightControl_Off,
        kDeviceLightControl_On,
        kDeviceLightControl_Control
    };

    enum IoTDeviceDoorCommand {
        kDeviceDoorControl_Close,
        kDeviceDoorControl_Open,
        kDeviceDoorControl_Bell,
    };

public:
    //  header
    bool            is_request_;
    std::string     command_;
    Json::Value     ext_info_;

    //  Device Control
    std::uint32_t   device_control_;

    //  Registration
    ccIoTDeviceSpecificationList    specification_info_list_;            //  Device Specfication
    std::string                     iot_device_master_uri_;

private:
    std::map < std::string, std::function<bool(const Json::Value& ext_info)>> command_map_;
};

#endif /* PRODUCTLIBRARY_CCIOTDEVICEPROTOCOLAPI_CCIOTDEVICEPROTOCOL_H_ */
