/*
 * Pi2IoTPositionSensor.cpp
 *
 *  Created on: 2016. 2. 4.
 *      Author: kmansoo
 */

#include "ccCoreAPI/ccCoreAPI.h"

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
        std::cout << "Unable to open serial device: " << strerror(errno) << std::endl;
    }
    else
    {
        _bIsStopThread = false;
        _oPollThread = std::thread(std::bind(&Pi2IoTPositionSensor::DoPoll, this));
    }

    if (wiringPiSetup() == -1)
        std::cout << "Unable to start wiringPi: " << strerror(errno) << std::endl;

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
        Luna::sleep(100);

        if (_nSerialFd != -1)
        {
#ifndef WIN32
            while (serialDataAvail(_nSerialFd))
#endif
            {
                char ch = serialGetchar(_nSerialFd);

                strRecvData += ch;

#ifdef WIN32
                strRecvData = "123 cm";
#endif

                if (ch == '\n')
                {
                    int nOffset = strRecvData.find("cm", 0);

                    if (std::string::npos != nOffset)
                        strRecvData.replace(nOffset, 2, "");

                    DoUpdateSesorData(strRecvData);
                }
                else
                    strRecvData += ch;
            }
        }
    }
}
