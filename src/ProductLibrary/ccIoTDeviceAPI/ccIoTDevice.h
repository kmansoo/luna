/*
 * ccIoTDevice.h
 *
 *  Created on: 2016. 1. 26.
 *      Author: kmansoo
 */

#ifndef PRODUCTLIBRARY_CCIOTDEVICEAPI_CCIOTDEVICE_H_
#define PRODUCTLIBRARY_CCIOTDEVICEAPI_CCIOTDEVICE_H_

#include <memory>

#include "ccCoreAPI/ccSingletonT.h"

#include "ccWebsocketClientAPI/ccEasyWebsocketClient.h"
#include "ccIoTDeviceProtocolAPI/ccIoTDeviceProtocol.h"

#include "ccIoTDeviceTransport.h"
//#include "ccIoTDeviceSpecification.h"
#include "ccIoTDeviceTransportFactory.h"

#include "ccJsonParserAPI/json/value.h"

class ccIoTDevice
{
public:
    ccIoTDevice();
    virtual ~ccIoTDevice();

public:
    bool    AttachFactory(std::shared_ptr<ccIoTDeviceTransportFactory> pFactory);
    bool    Start(const std::string& strIoTManagerUri);
    bool    Stop();

    bool    Send(ccIoTDeviceProtocol& oProtocol);

public:
    virtual bool    OnRecvCommand(ccIoTDeviceProtocol& oProtocol);

protected:
    bool    DoSetDeviceCommand(ccIoTDeviceProtocol& oProtocol);
    bool    DoGetDeviceStatusCommand(ccIoTDeviceProtocol& oProtocol);

protected:
    void    DoRetryConnect();
    void    DoRecvDataFromWebsocket(ccWebsocket::ccWebSocketEvent eEvent, const std::string& message);

protected:
    std::string             _strTargetUri;
    ccEasyWebsocketClient   _oWSC;
    std::shared_ptr<ccIoTDeviceTransportFactory> _pFactory;

    std::map < std::string, std::function<bool(ccIoTDeviceProtocol& oProtocol)>> _aCommands;
};

#endif /* PRODUCTLIBRARY_CCIOTDEVICEAPI_CCIOTDEVICE_H_ */
