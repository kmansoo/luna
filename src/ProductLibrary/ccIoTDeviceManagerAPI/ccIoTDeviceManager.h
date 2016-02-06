/*
 * ccRESTfulChattingApi.h
 *
 *  Created on: 2015. 11. 28.
 *      Author: kmansoo
 */

#ifndef CCPRODUCTLIBRARY_CCIOTDEVICEMANAGERAPI_CCIOTDEVICEMANAGER_H_
#define CCPRODUCTLIBRARY_CCIOTDEVICEMANAGERAPI_CCIOTDEVICEMANAGER_H_

#include <string>
#include <functional>
#include <memory>
#include <map>
#include <mutex>

#include "ccWebServerAPI/ccRESTfulApi.h"
#include "ccWebServerAPI/ccWebsocketManager.h"

#include "ccIoTDeviceProtocolAPI/ccIoTDeviceProtocol.h"

#include "ccIoTDeviceAgent.h"

class ccIoTDeviceManager
{
public:
    ccIoTDeviceManager();
    virtual ~ccIoTDeviceManager();

public:
    void    Show();
    void    AllSwitchesControl(bool bOn);
    void    AllLightsControl(bool bOn);
    void    AllLocksControl(bool bOpen);

protected:
    //  RESTful API Function
    bool    devices(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse);
    bool    devices_device(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse);

protected:
    //  Websocket Function
    bool    ws_IoTDevice(ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<ccWebsocket> pWS, const std::string strData);

protected:
    bool        DoFindeDeviceAgent(std::shared_ptr<ccWebsocket> pWS, std::shared_ptr<ccIoTDeviceAgent>& pDA);
    bool        DoParseCommand(std::shared_ptr<ccWebsocket> pWS, const std::string strData);

protected:
    virtual bool    DoRegisterCommand(std::shared_ptr<ccWebsocket> pWS, ccIoTDeviceProtocol& oProtocol);
    virtual bool    DoDeRegisterCommand(std::shared_ptr<ccWebsocket> pWS, ccIoTDeviceProtocol& oProtocol);
    virtual bool    DoGetDeviceStatusCommand(std::shared_ptr<ccWebsocket> pWS, ccIoTDeviceProtocol& oProtocol);
    virtual bool    DoUpdateDeviceStatusCommand(std::shared_ptr<ccWebsocket> pWS, ccIoTDeviceProtocol& oProtocol);
    virtual bool    DoSetControlCommand(std::shared_ptr<ccWebsocket> pWS, ccIoTDeviceProtocol& oProtocol);

protected:
    std::map < std::int32_t, std::shared_ptr<ccIoTDeviceAgent> > _aAgents;

    std::mutex  _mtx;

    std::shared_ptr<ccRESTfulApi>       _oWebAPI;
    std::shared_ptr<ccWebsocketManager> _oWSM;

    std::map < std::string, std::function<bool(std::shared_ptr<ccWebsocket> pWS, ccIoTDeviceProtocol& oProtocol)>> _aCommands;
};


#endif /* CCPRODUCTLIBRARY_CCIOTDEVICEMANAGERAPI_CCIOTDEVICEMANAGER_H_ */
