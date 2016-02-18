/*
 * Pi2IoTPositionSensor.cpp
 *
 *  Created on: 2016. 2. 4.
 *      Author: kmansoo
 */

#include "ccCoreAPI/ccCoreAPI.h"
#include "ccCoreAPI/ccString.h"

#include "Pi2IoTPositionSensor.h"
#include "Pi2GpioControlManager.h"

#include <iostream>

#ifdef LUNA_PLATFORM_RASPBERRY_PI2

#   include <wiringPi.h>

#else
 //
 //  To avoid compiler error on Win32, I had made fake functions as following.
 //
int     serialOpen(char*, int) { return 1; }
int     wiringPiSetup() { return true; }
bool    serialDataAvail(int) { return true; }
char    serialGetchar(int) { return '\n'; }
long    micros() 
{
    return (long)Luna::getTickCount();
}

#endif // !WIN32


Pi2IoTPositionSensor::Pi2IoTPositionSensor() : ccIoTDevice("MyDeviceInfo.json")
{
    _bIsStopThread  = false;

    // set GPIO
    Pi2GpioControlManager::getInstance().SetPinMode(kUltraSonicPingSensor_TriggerPin, kGpioPinMode_OUTPUT); // for Trigger Pin
    Pi2GpioControlManager::getInstance().SetPinMode(kUltraSonicPingSensor_EchoPin, kGpioPinMode_INPUT);     // for Echo Pin
}

Pi2IoTPositionSensor::~Pi2IoTPositionSensor()
{
    // TODO Auto-generated destructor stub
}

bool Pi2IoTPositionSensor::Start()
{
    _bIsStopThread = false;
    _oPollThread = std::thread(std::bind(&Pi2IoTPositionSensor::DoPoll, this));

    if (wiringPiSetup() == -1)
        std::cout << "Unable to start wiringPi: " << std::endl;

    return ccIoTDevice::Start();
}

bool Pi2IoTPositionSensor::Stop()
{
    _bIsStopThread = true;
    _oPollThread.join();

    return ccIoTDevice::Stop();
}

void Pi2IoTPositionSensor::DoMeasureLength()
{
    //raw duration in milliseconds, cm is the
    //converted amount into a distance
    long lDuration;

    //sending the signal, starting with LOW for a clean signal
    Pi2GpioControlManager::getInstance().Set(kUltraSonicPingSensor_TriggerPin, kLunaGpioValue_LOW);
    Luna::sleep(30);
    Pi2GpioControlManager::getInstance().Set(kUltraSonicPingSensor_TriggerPin, kLunaGpioValue_HIGH);
    Luna::usleep(20);
    Pi2GpioControlManager::getInstance().Set(kUltraSonicPingSensor_TriggerPin, kLunaGpioValue_LOW);

    long startTime = 0, endTime = 0;

    while (Pi2GpioControlManager::getInstance().Get(kUltraSonicPingSensor_EchoPin) == 0);
    startTime = micros();

    while (Pi2GpioControlManager::getInstance().Get(kUltraSonicPingSensor_EchoPin) == 1);
    endTime = micros();

    lDuration = endTime - startTime; //enPi2GpioControlManager::getInstance().Get(kUltraSonicPingSensor_EchoPin);

    float fDistance = (float)lDuration / 58.0f;

    std::string strFormatedData;

    ccString::format(strFormatedData, "%5.2f", fDistance);

    DoUpdateSesorData(strFormatedData);

    //  printf("Duration = %d(%5.2f cm)\n", fDuration, fDistance);
}

bool Pi2IoTPositionSensor::DoUpdateSesorData(std::string& strData)
{
    ccIoTDeviceProtocol oControlProtocol;

    Json::Value oExtInfo;

    oExtInfo["DevceType"]   = "Sensor";
    oExtInfo["Value"]       = strData;

    oControlProtocol._IsRequest = true;
    oControlProtocol._strCommand = "UpdateDeviceStatus";
    oControlProtocol._oExtInfo = oExtInfo;

    Send(oControlProtocol);

    return true;
}

void Pi2IoTPositionSensor::DoPoll()
{
    std::string strRecvData;

    while (_bIsStopThread == false)
    {
        Luna::sleep(100);

#ifdef LUNA_PLATFORM_RASPBERRY_PI2
        {
            DoMeasureLength();
        }
#else
        {
            // Send the fake data to test.
            ccString::format(strRecvData, "%d", rand() % 20 + 50);
            DoUpdateSesorData(strRecvData);
            Luna::sleep(500);
        }
#endif
    }
}
