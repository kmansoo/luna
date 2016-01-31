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

#include "ccJsonParserAPI/json/value.h"

#include "ccWebsocketClientAPI/ccEasyWebsocketClient.h"

#include "ccIoTDeviceProtocolAPI/ccIoTDeviceProtocol.h"
#include "ccIoTDeviceProtocolAPI/ccIoTDeviceSpecification.h"

#include "ccIoTDeviceTransport.h"
#include "ccIoTDeviceTransportFactory.h"

class ccIoTDevice
{
public:
    ccIoTDevice(const std::string& strSpecFile = "MyDeviceInfo.json");
    virtual ~ccIoTDevice();

public:
    bool    AttachFactory(std::shared_ptr<ccIoTDeviceTransportFactory> pFactory);
    bool    Start(const std::string& strIoTManagerUri);
    bool    Stop();

    bool    Send(ccIoTDeviceProtocol& oProtocol);

public:
    virtual bool    OnRecvCommand(ccIoTDeviceProtocol& oProtocol);

protected:
    virtual bool    DoSetDeviceCommand(ccIoTDeviceProtocol& oProtocol);
    virtual bool    DoGetDeviceStatusCommand(ccIoTDeviceProtocol& oProtocol);

protected:
    void    DoRetryConnect();
    void    DoRecvDataFromWebsocket(ccWebsocket::ccWebSocketEvent eEvent, const std::string& message);

protected:
    bool                        _bIsConnected;
    bool                        _bIsStopByUser;

    ccIoTDeviceSpecification    _oSpecification;
    std::string                 _strTargetUri;
    ccEasyWebsocketClient       _oWSC;
    std::shared_ptr<ccIoTDeviceTransportFactory> _pFactory;

    std::map < std::string, std::function<bool(ccIoTDeviceProtocol& oProtocol)>> _aCommands;
};

#endif /* PRODUCTLIBRARY_CCIOTDEVICEAPI_CCIOTDEVICE_H_ */
