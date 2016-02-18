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
#include <vector>

#include "ccWebServerAPI/ccWebsocket.h"
#include "ccJsonParserAPI/json/value.h"

#include "ccIoTDeviceProtocolAPI/ccIoTDeviceSpecificationList.h"

class ccIoTDeviceAgent
{
public:
    ccIoTDeviceAgent(std::shared_ptr<ccWebsocket> pWS, ccIoTDeviceSpecificationList&& oSpecInfo);
    virtual ~ccIoTDeviceAgent();

public:
    void    Show();

public:
    int GetDeviceCount() { return _oDeviceSpecInfo.Size(); }
    ccIoTDeviceSpecificationList&   GetDeviceSpecInfo() { return _oDeviceSpecInfo; }    //  for All Device Info

    bool    HasDevice(ccIoTDeviceSpecification::IoTDeviceType eDeviceType);
    bool    HasDevice(const std::string& strDeviceName);

    int     GetDeviceIndex(ccIoTDeviceSpecification::IoTDeviceType eDeviceType);

    ccIoTDeviceSpecification&       GetDeviceSpec(std::size_t nIndex = 0);  //  for Specific Device

    ccIoTDeviceSpecification::IoTDeviceType GetType(std::size_t nIndex = 0);
    const std::string&                  GetTypeName(std::size_t nIndex = 0);
    const std::string&                  GetName(std::size_t nIndex = 0);

    std::int32_t                        GetID() { return _pWS->GetInstance(); }
    const std::shared_ptr<ccWebsocket>  GetWebsockt() {return _pWS; }

private:
    std::shared_ptr<ccWebsocket>    _pWS;
    ccIoTDeviceSpecificationList    _oDeviceSpecInfo;

    ccIoTDeviceSpecification        _oNullObj;
    std::string                     _oNullData;

};

#endif /* PRODUCTLIBRARY_CCIOTDEVICEMANAGERAPI_CCIOTDEVICEAGENT_H_ */
