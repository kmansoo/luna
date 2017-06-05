/*
 * Pi2IoTLightDevice.h
 *
 *  Created on: 2016. 2. 4.
 *      Author: kmansoo
 */

#ifndef RASPBERRYPIAPPS_PI2IOTLIGHTDEVICE_SRC_PI2IOTLIGHTDEVICE_H_
#define RASPBERRYPIAPPS_PI2IOTLIGHTDEVICE_SRC_PI2IOTLIGHTDEVICE_H_

#include "ccIoTDevice/ccIoTDevice.h"

class Pi2IoTLightDevice : public ccIoTDevice
{
public:
    Pi2IoTLightDevice();
    virtual ~Pi2IoTLightDevice();

protected:
    virtual bool    DoSetDeviceCommand(ccIoTDeviceProtocol& oProtocol);
    virtual bool    DoGetDeviceStatusCommand(ccIoTDeviceProtocol& oProtocol);

};

#endif /* RASPBERRYPIAPPS_PI2IOTLIGHTDEVICE_SRC_PI2IOTLIGHTDEVICE_H_ */
