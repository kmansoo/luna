/*
 * Pi2IoTPositionSensor.cpp
 *
 *  Created on: 2016. 2. 4.
 *      Author: kmansoo
 */

#include "ccCoreAPI/ccCoreAPI.h"
#include "ccCoreAPI/ccString.h"

#include "Pi2IoTPositionSensor.h"

#include <iostream>

#ifndef WIN32

#   include <wiringPi.h>
#   include <wiringSerial.h>
#else
 //
 //  To avoid compiler error on Win32, I had made fake functions as following.
 //
int     serialOpen(char*, int) { return 1; }
int     wiringPiSetup() { return true; }
bool    serialDataAvail(int) { return true; }
char    serialGetchar(int) { return '\n'; }

#endif // !WIN32


Pi2IoTPositionSensor::Pi2IoTPositionSensor() : ccIoTDevice("MyDeviceInfo.json")
{
    _bIsStopThread  = false;
    _nSerialFd = -1;

}

Pi2IoTPositionSensor::~Pi2IoTPositionSensor()
{
    // TODO Auto-generated destructor stub
}

bool Pi2IoTPositionSensor::Start()
{
    if ((_nSerialFd = serialOpen("/dev/ttyACM0", 9600)) < 0)    //  ttyAMA0
    {
        std::cout << "Unable to open serial device: " << std::endl;
    }
    else
    {
        _bIsStopThread = false;
        _oPollThread = std::thread(std::bind(&Pi2IoTPositionSensor::DoPoll, this));
    }

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

bool Pi2IoTPositionSensor::DoUpdateSesorData(std::string& strData)
{
    ccIoTDeviceProtocol oControlProtocol;

    Json::Value oExtInfo;

    oExtInfo["Value"] = strData;

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
        Luna::sleep(10);

        if (_nSerialFd != -1)
        {
 #ifdef LUNA_PLATFORM_RASPBERRY_PI2
            while (serialDataAvail(_nSerialFd))
            {
                char ch = serialGetchar(_nSerialFd);

                // for debuging
                // printf("%c[%d]", ch, ch);
                // fflush(stdout);
                //
                //  Arduino --> Raspberry Pi2 : 'xxx cm\n'
                //
                //  The following characters are not required. So those are skipping.
                if (ch == ' ' || ch == 'c' || ch == 'm')
                    continue;

                if (ch == '\n')
                {
                    if (strRecvData.length() > 0)
                        DoUpdateSesorData(strRecvData);

                    strRecvData = "";
                }
                else
                    strRecvData += ch;
            }
#elif defined(WIN32)
            // Send the fake data to test.
            ccString::format(strRecvData, "%d", rand() % 10 + 50);
            DoUpdateSesorData(strRecvData);
            Luna::sleep(500);
#endif
        }
    }
}
