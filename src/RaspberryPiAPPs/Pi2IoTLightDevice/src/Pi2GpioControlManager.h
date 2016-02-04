/*
 * Pi2GpioControlManager.h
 *
 *  Created on: 2016. 2. 4.
 *      Author: kmansoo
 */

#ifndef RASPBERRYPIAPPS_PI2IOTLIGHTDEVICE_PI2GPIOCONTROLMANAGER_H_
#define RASPBERRYPIAPPS_PI2IOTLIGHTDEVICE_PI2GPIOCONTROLMANAGER_H_

#include <cstdint>
#include "ccCoreAPI/ccSingletonT.h"

enum LunaGpioPinMode {
    kGpioPinMode_INPUT = 0,
    kGpioPinMode_OUTPUT = 1,
    kGpioPinMode_PWM_OUTPUT = 2,
    kGpioPinMode_GPIO_CLOCK = 3,
};

enum LunaGpioValue {
    kLunaGpioValue_LOW      =   0,
    kLunaGpioValue_HIGH     =   1,
};

class Pi2GpioControlManager : public ccSingletonT<Pi2GpioControlManager>
{
public:
    virtual ~Pi2GpioControlManager();

protected:
    Pi2GpioControlManager();

    friend class ccSingletonT<Pi2GpioControlManager>;

public:
    bool            SetPinMode(std::uint32_t pin, LunaGpioPinMode eMode);

    bool            Set(std::uint32_t pin, std::int32_t value);
    std::int32_t    Get(std::uint32_t pin);
};

#endif /* RASPBERRYPIAPPS_PI2IOTLIGHTDEVICE_PI2GPIOCONTROLMANAGER_H_ */
