/*
 * ccIoTDeviceAgent.h
 *
 *  Created on: 2016. 1. 28.
 *      Author: kmansoo
 */

#ifndef PRODUCTLIBRARY_CCIOTDEVICEMANAGERAPI_CCIOTDEVICEAGENT_H_
#define PRODUCTLIBRARY_CCIOTDEVICEMANAGERAPI_CCIOTDEVICEAGENT_H_

#include <string>
#include <memory>

#include "ccWebServerAPI/ccWebsocket.h"
#include "ccJsonParserAPI/json/value.h"

#include "ccIoTDeviceProtocolAPI/ccIoTDeviceSpecification.h"

class ccIoTDeviceAgent
{
public:
    ccIoTDeviceAgent(std::shared_ptr<ccWebsocket> pWS, const Json::Value& oSpec);
    virtual ~ccIoTDeviceAgent();

public:
    void    Show();

public:
    ccIoTDeviceSpecification::IoTDeviceType GetType() { return _oSpecification.GetType(); }
    const std::string&                  GetTypeName() { return _oSpecification.GetTypeName(); }

    std::int32_t                        GetID() { return _pWS->GetInstance(); }
    const std::string&                  GetName() {return _oSpecification.GetName(); }
    const std::shared_ptr<ccWebsocket>  GetWebsockt() {return _pWS; }
    ccIoTDeviceSpecification&           GetDeviceSPec() { return _oSpecification; }

private:
    std::shared_ptr<ccWebsocket>    _pWS;
    ccIoTDeviceSpecification        _oSpecification;

};

#endif /* PRODUCTLIBRARY_CCIOTDEVICEMANAGERAPI_CCIOTDEVICEAGENT_H_ */
