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

class ccIoTDeviceAgent
{
public:
    ccIoTDeviceAgent(std::shared_ptr<ccWebsocket> pWS);
    virtual ~ccIoTDeviceAgent();

public:
    const std::string& GetName() {return _strName; }
    const std::shared_ptr<ccWebsocket> GetWebsockt() {return _pWS; }

private:
    std::string                     _strName;
    std::shared_ptr<ccWebsocket>    _pWS;
};

#endif /* PRODUCTLIBRARY_CCIOTDEVICEMANAGERAPI_CCIOTDEVICEAGENT_H_ */
