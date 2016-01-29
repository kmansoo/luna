/*
 * ccIoTDevice.cpp
 *
 *  Created on: 2016. 1. 26.
 *      Author: kmansoo
 */

#include <iostream>

#include "ccCoreAPI/ccTimer.h"

#include "ccIoTDevice.h"

ccIoTDevice::ccIoTDevice()
{
    _aCommands["SetDevice"] = std::bind(&ccIoTDevice::DoSetDeviceCommand, this, std::placeholders::_1);
    _aCommands["GetDeviceStatus"] = std::bind(&ccIoTDevice::DoGetDeviceStatusCommand, this, std::placeholders::_1);
}

ccIoTDevice::~ccIoTDevice()
{
	// TODO Auto-generated destructor stub
}


bool ccIoTDevice::AttachFactory(std::shared_ptr<ccIoTDeviceTransportFactory> pFactory)
{

    return false;
}

bool ccIoTDevice::Start(const std::string& strIoTManagerUri)
{
    _strTargetUri = std::move(strIoTManagerUri);

    _oWSC.SetEventListener(std::bind(&ccIoTDevice::DoRecvDataFromWebsocket, this, std::placeholders::_1, std::placeholders::_2));

    bool bRetVal = _oWSC.Open(_strTargetUri);

    if (bRetVal == false)
    {
        _oWSC.Close();

        ccTimer oRetryTimer(5000, true, std::bind(&ccIoTDevice::DoRetryConnect, this));
    }

    return bRetVal;
}

bool ccIoTDevice::Stop()
{
    return _oWSC.Close();
}

bool ccIoTDevice::Send(ccIoTDeviceProtocol& oProtocol)
{
    return oProtocol.Send(&_oWSC);
}

//  virtual functions
bool ccIoTDevice::OnRecvCommand(ccIoTDeviceProtocol& oProtocol)
{
    std::cout << "ccIoTDevice: OnRecvCommand, " << std::endl;

    auto it = _aCommands.find(oProtocol._strCommand);

    if (it != std::end(_aCommands))
        it->second(oProtocol);

    return true;
}

void ccIoTDevice::DoRetryConnect()
{
    std::cout << "ccIoTDevice: DoRetryConnect, " << std::endl;

    _oWSC.Close();

    if (_oWSC.Open(_strTargetUri) == false)
    {
        ccTimer oRetryTimer(5000, true, std::bind(&ccIoTDevice::DoRetryConnect, this));
    }
}

//  
void ccIoTDevice::DoRecvDataFromWebsocket(ccWebsocket::ccWebSocketEvent eEvent, const std::string& message)
{
    switch ((int)eEvent)
    {
    case ccWebsocket::ccWebSocketEvent_Connected:
        std::cout << "ccIoTDevice: Connected" << std::endl;
        break;

    case ccWebsocket::ccWebSocketEvent_Disconnected:
        {
            std::cout << "ccIoTDevice: Disconnected" << std::endl;

            ccTimer oRetryTimer(5000, true, std::bind(&ccIoTDevice::DoRetryConnect, this));
        }
        break;

    case ccWebsocket::ccWebSocketEvent_ReceivedData:
        {
            std::cout << "ccIoTDevice: ReceivedData, " << message << std::endl;

            ccIoTDeviceProtocol oProtocol;

            if (oProtocol.Parse(message))
                OnRecvCommand(oProtocol);
        }
        break;
    }
}

bool ccIoTDevice::DoSetDeviceCommand(ccIoTDeviceProtocol& oProtocol)
{
    if (oProtocol._IsRequest == false)
        return false;

    std::cout << "ccIoTDevice: DoSetDeviceCommand " << std::endl;

    ccIoTDeviceProtocol oResponseProtocol;

    oResponseProtocol.Send(&_oWSC, false, oProtocol._strCommand);

    return true;
}

bool ccIoTDevice::DoGetDeviceStatusCommand(ccIoTDeviceProtocol& oProtocol)
{
    if (oProtocol._IsRequest == false)
        return false;

    std::cout << "ccIoTDevice: DoGetDeviceStatusCommand " << std::endl;

    ccIoTDeviceProtocol oResponseProtocol;
    oResponseProtocol.Send(&_oWSC, false, oProtocol._strCommand);

    return true;
}
