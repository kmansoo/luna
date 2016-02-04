/*
 * Pi2IoTPositionSensor.h
 *
 *  Created on: 2016. 2. 4.
 *      Author: kmansoo
 */

#include <thread>

#ifndef RASPBERRYPIAPPS_PI2IOTLIGHTDEVICE_SRC_PI2IOTPOSITIONSENSOR_H_
#define RASPBERRYPIAPPS_PI2IOTLIGHTDEVICE_SRC_PI2IOTPOSITIONSENSOR_H_

#include "ccIoTDeviceAPI/ccIoTDevice.h"

class Pi2IoTPositionSensor : public ccIoTDevice
{
public:
    Pi2IoTPositionSensor();
    virtual ~Pi2IoTPositionSensor();

public:
    virtual bool    Start();
    virtual bool    Stop();

private:
    void    DoPoll();
    bool    DoUpdateSesorData(std::string& strData);

private:
    std::thread _oPollThread;
    bool        _bIsStopThread;

    int         _nSerialFd;
};

#endif /* RASPBERRYPIAPPS_PI2IOTLIGHTDEVICE_SRC_PI2IOTPOSITIONSENSOR_H_ */
