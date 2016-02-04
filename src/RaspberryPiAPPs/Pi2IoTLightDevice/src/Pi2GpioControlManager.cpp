/*
 * Pi2GpioControlManager.cpp
 *
 *  Created on: 2016. 2. 4.
 *      Author: kmansoo
 */

#include <iostream>

#include "Pi2GpioControlManager.h"

#ifndef WIN32

// To install the wiringPi.h, please refer to http://wiringpi.com/reference/
// wiringPi : GPIO Interface library for the Raspberry Pi
#include <wiringPi.h>

#else
//
//  To avoid compiler error on Win32, I had made fake functions as following.
//
#define INPUT       kGpioPinMode_INPUT
#define OUTPUT      kGpioPinMode_OUTPUT
#define PWM_OUTPUT  kGpioPinMode_PWM_OUTPUT
#define GPIO_CLOCK  kGpioPinMode_GPIO_CLOCK

void    wiringPiSetup() {}
void    pinMode(std::uint32_t, int) {}
void    digitalWrite(std::uint32_t, int) {}
int     digitalRead(std::uint32_t) { return 0; }

#endif

Pi2GpioControlManager::Pi2GpioControlManager()
{
    // To install the wiringPi.h, please refer to http://wiringpi.com/reference/
    // wiringPi : GPIO Interface library for the Raspberry Pi
    wiringPiSetup();
}

Pi2GpioControlManager::~Pi2GpioControlManager()
{
    // TODO Auto-generated destructor stub
}

bool Pi2GpioControlManager::SetPinMode(std::uint32_t pin, LunaGpioPinMode eMode)
{
    int aWiringPinMode[] = {
            INPUT,
            OUTPUT,
            PWM_OUTPUT,
            GPIO_CLOCK,
    };

    pinMode(pin, aWiringPinMode[(int)eMode]);

    return true;
}

bool Pi2GpioControlManager::Set(std::uint32_t pin, std::int32_t value)
{
    digitalWrite(pin, value);

    return true;
}

std::int32_t Pi2GpioControlManager::Get(std::uint32_t pin)
{
    return digitalRead(pin);
}
