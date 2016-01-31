/*
 * ccIoTDevice.cpp
 *
 *  Created on: 2016. 1. 26.
 *      Author: kmansoo
 */

#include <iostream>

#include "ccCoreAPI/ccTimer.h"

#include "ccIoTDevice.h"

ccIoTDevice::ccIoTDevice(const std::string& strSpecFile) : _bIsConnected(false), _bIsStopByUser(false)
{
    _aCommands["SetDevice"] = std::bind(&ccIoTDevice::DoSetDeviceCommand, this, std::placeholders::_1);
    _aCommands["GetDeviceStatus"] = std::bind(&ccIoTDevice::DoGetDeviceStatusCommand, this, std::placeholders::_1);

    _oWSC.SetEventListener(std::bind(&ccIoTDevice::DoRecvDataFromWebsocket, this, std::placeholders::_1, std::placeholders::_2));

    //  ÀÐ±â
    _oSpecification.LoadFile(strSpecFile);
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
    if (_bIsConnected)
        return true;

    _bIsStopByUser = false;

    _strTargetUri = std::move(strIoTManagerUri);

    DoRetryConnect();

    return true;
}

bool ccIoTDevice::Stop()
{
    _bIsStopByUser = true;

    if (!_bIsConnected)
        return true;

    ccIoTDeviceProtocol oProtocol;
    oProtocol.Send(&_oWSC, true, "DeRegister");

    //  ccTimer oRetryTimer(500, true, std::bind(&ccIoTDevice::DoCloseWS, this));

    _bIsConnected = false;

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
    if (_bIsStopByUser)
        return;

    std::cout << "ccIoTDevice: DoRetryConnect, " << std::endl;

    _oWSC.Close();

    if (_oWSC.Open(_strTargetUri) == false)
    {
        ccTimer oRetryTimer(5000, true, std::bind(&ccIoTDevice::DoRetryConnect, this));
    }

    _bIsConnected = true;

    ccIoTDeviceProtocol oProtocol;
    oProtocol.Send(&_oWSC, true, "Register", _oSpecification.ToJson());
}

//void ccIoTDevice::DoCloseWS()
//{
//
//}

//  
void ccIoTDevice::DoRecvDataFromWebsocket(ccWebsocket::ccWebSocketEvent eEvent, const std::string& message)
{
    switch ((int)eEvent)
    {
    case ccWebsocket::ccWebSocketEvent_Connected:
        {
            _bIsConnected = true;
            ccIoTDeviceProtocol oProtocol;
            oProtocol.Send(&_oWSC, true, "Register", _oSpecification.ToJson());

            std::cout << "ccIoTDevice: Connected" << std::endl;
        }
        break;

    case ccWebsocket::ccWebSocketEvent_Disconnected:
        {
            _bIsConnected = false;

            std::cout << "ccIoTDevice: Disconnected" << std::endl;

            if (!_bIsStopByUser)
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

    //std::cout << "ccIoTDevice: DoSetDeviceCommand " << std::endl;
    //ccIoTDeviceProtocol oResponseProtocol;
    //oResponseProtocol.Send(&_oWSC, false, oProtocol._strCommand);

    return true;
}

bool ccIoTDevice::DoGetDeviceStatusCommand(ccIoTDeviceProtocol& oProtocol)
{
    if (oProtocol._IsRequest == false)
        return false;

    //std::cout << "ccIoTDevice: DoGetDeviceStatusCommand " << std::endl;
    //ccIoTDeviceProtocol oResponseProtocol;
    //oResponseProtocol.Send(&_oWSC, false, oProtocol._strCommand);

    return true;
}
