/*
 * Pi2IoTLightDevice.cpp
 *
 *  Created on: 2016. 2. 4.
 *      Author: kmansoo
 */

#include <iostream>

#include "Pi2IoTLightDevice.h"

#include "Pi2GpioControlManager.h"

Pi2IoTLightDevice::Pi2IoTLightDevice() : ccIoTDevice("MyDeviceInfo.json")
{
    Pi2GpioControlManager::getInstance().SetPinMode(1, kGpioPinMode_OUTPUT);
}

Pi2IoTLightDevice::~Pi2IoTLightDevice()
{
    // TODO Auto-generated destructor stub
}

bool Pi2IoTLightDevice::DoSetDeviceCommand(ccIoTDeviceProtocol& oProtocol)
{
    if (oProtocol._oExtInfo.isNull())
        return false;

    //  이곳에서 GPiO를 제어하면 됩니다.
    if (oProtocol._oExtInfo["Control"] == "On")
    {
        std::cout << "Pi2IoTLightDevice, LED --> Turn On" << std::endl;

        Pi2GpioControlManager::getInstance().Set(1, kLunaGpioValue_HIGH);
    }
    else
    {
        std::cout << "Pi2IoTLightDevice, LED --> Turn off" << std::endl;

        Pi2GpioControlManager::getInstance().Set(1, kLunaGpioValue_LOW);
    }

    return true;
}

bool Pi2IoTLightDevice::DoGetDeviceStatusCommand(ccIoTDeviceProtocol& oProtocol)
{


    return false;
}
