/*
 * Pi2IoTPositionServiceManager.h
 *
 *  Created on: 2016. 2. 4.
 *      Author: kmansoo
 */

#ifndef RASPBERRYPIAPPS_PI2IOTSERVICEMANAGERBASEDPOSITIONSENSOR_SRC_PI2IOTPOSITIONSERVICEMANAGER_H_
#define RASPBERRYPIAPPS_PI2IOTSERVICEMANAGERBASEDPOSITIONSENSOR_SRC_PI2IOTPOSITIONSERVICEMANAGER_H_

#include "ccIoTDeviceManagerAPI/ccIoTDeviceManager.h"

class Pi2IoTPositionServiceManager : public ccIoTDeviceManager
{
public:
    Pi2IoTPositionServiceManager();
    virtual ~Pi2IoTPositionServiceManager();

protected:
    virtual bool    DoUpdateDeviceStatusCommand(std::shared_ptr<ccWebsocket> pWS, ccIoTDeviceProtocol& oProtocol);
};

#endif /* RASPBERRYPIAPPS_PI2IOTSERVICEMANAGERBASEDPOSITIONSENSOR_SRC_PI2IOTPOSITIONSERVICEMANAGER_H_ */
