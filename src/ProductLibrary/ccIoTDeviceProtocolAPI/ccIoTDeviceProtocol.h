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
#include <map>

#include "ccJsonParserAPI/json/value.h"

#include "ccWebServerAPI/ccWebsocket.h"

class ccIoTDeviceProtocol
{
public:
    ccIoTDeviceProtocol();
    virtual ~ccIoTDeviceProtocol();

public:
    bool    Send(ccWebsocket* pWS);
    bool    Send(ccWebsocket* pWS, bool bIsRequest, const std::string& strCommand);
    bool    Send(ccWebsocket* pWS, bool bIsRequest, const std::string& strCommand, const Json::Value& oExtInfo);

    bool    Parse(const std::string& strData);

protected:
    bool    DoParserRegisterCommand(const Json::Value& oExtInfo);
    bool    DoParserDeRegisterCommand(const Json::Value& oExtInfo);
    bool    DoParserSetDeviceCommand(const Json::Value& oExtInfo);
    bool    DoParserGetDeviceStatusCommand(const Json::Value& oExtInfo);

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
    bool            _IsRequest;
    std::string     _strCommand;
    Json::Value     _oExtInfo;

    //  Device COntrol
    std::uint32_t   _uDeviceControl;

private:
    std::map < std::string, std::function<bool(const Json::Value& oExtInfo)>> _aParsers;
};

#endif /* PRODUCTLIBRARY_CCIOTDEVICEPROTOCOLAPI_CCIOTDEVICEPROTOCOL_H_ */
