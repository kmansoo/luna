/*
 * ccRESTfulWebServer.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include <iostream>
#include <functional>

#include "ccJsonParserAPI/json/value.h"
#include "ccJsonParserAPI/json/writer.h"
#include "ccJsonParserAPI/json/reader.h"

#include "ccWebServerAPI/ccWebServerManager.h"

#include "ccMongooseServerAPI/ccMongooseWebServerObjectFactory.h"

#include "ccIoTDeviceManager.h"

ccIoTDeviceManager::ccIoTDeviceManager()
{
    _oWebAPI.reset(new ccRESTfulApi);
    _oWSM.reset(new ccWebsocketManager);

    //  add commands
    _aCommands["Register"] = std::bind(&ccIoTDeviceManager::DoRegisterCommand, this, std::placeholders::_1, std::placeholders::_2);
    _aCommands["DeRegister"] = std::bind(&ccIoTDeviceManager::DoDeRegisterCommand, this, std::placeholders::_1, std::placeholders::_2);
    _aCommands["GetDeviceStatus"] = std::bind(&ccIoTDeviceManager::DoGetDeviceStatusCommand, this, std::placeholders::_1, std::placeholders::_2);
    _aCommands["UpdateDeviceStatus"] = std::bind(&ccIoTDeviceManager::DoUpdateDeviceStatusCommand, this, std::placeholders::_1, std::placeholders::_2);

    // TODO Auto-generated constructor stub
    _oWebAPI->AddAPI(std::string("/devices"), std::bind(&ccIoTDeviceManager::devices, this, std::placeholders::_1, std::placeholders::_2));
    _oWSM->AddFunction("/ws_iot_deivce", std::bind(&ccIoTDeviceManager::ws_IoTDevice, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    ccWebServerManager::getInstance().AttachFactory(std::make_shared<ccMongooseWebServerObjectFactory>());
    ccWebServerManager::getInstance().CreateWebServer("WebServer #1", "8000", "C:\\web_root\\Chatting\\");

    ccWebServerManager::getInstance().AddRESTfulApi(_oWebAPI);
    ccWebServerManager::getInstance().AddWebsocketManager(_oWSM);

    ccWebServerManager::getInstance().Start();
}

ccIoTDeviceManager::~ccIoTDeviceManager()
{
    // TODO Auto-generated destructor stub
}

//  Open API
bool ccIoTDeviceManager::devices(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)
{
    //std::cout << "enter ccRESTfulChatting::list, query_string=" << pRequest->GetQueryString() << endl;

    switch (pRequest->GetMethod())
    {
    case ccWebServerRequest::HttpMethod_Post:   //  Create
        break;

    case ccWebServerRequest::HttpMethod_Get:    //  Read
        break;

    case ccWebServerRequest::HttpMethod_Delete:   //  Delete
        break;

    default:
        break;
    }

    return false;
}

//  WebSocket
bool ccIoTDeviceManager::ws_IoTDevice(ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<ccWebsocket> pWS, const std::string strData)
{
    std::cout << std::endl;

    switch ((int)eEvent)
    {
    case ccWebsocket::ccWebSocketEvent_Connected:
        std::cout << "ccIoTDeviceManager: An IoT device is connected. { " << std::endl << strData << std::endl << "}" << std::endl;
        break;

    case ccWebsocket::ccWebSocketEvent_ReceivedData:
        std::cout << "ccIoTDeviceManager: We got the ommmand from IoT Device. { " << std::endl << strData << std::endl << "}" << std::endl;
        DoParseCommand(pWS, strData);
        break;

    case ccWebsocket::ccWebSocketEvent_Disconnected:
        std::cout << "ccIoTDeviceManager: An IoT device is disconnected." << std::endl;
        {
            ccIoTDeviceProtocol oNoProtocol;

            oNoProtocol._IsRequest = true;

            DoDeRegisterCommand(pWS, oNoProtocol);
        }
        break;
    }

    return true;
}

bool  ccIoTDeviceManager::DoParseCommand(std::shared_ptr<ccWebsocket> pWS, const std::string strData)
{
    ccIoTDeviceProtocol oProtocol;
    if (oProtocol.Parse(strData))
    {
        if (oProtocol._IsRequest)
            std::cout << "ccIoTDevice Request Command: " << oProtocol._strCommand << std::endl;
        else
            std::cout << "ccIoTDevice Response Command: " << oProtocol._strCommand << std::endl;

        auto it = _aCommands.find(oProtocol._strCommand);

        if (it != std::end(_aCommands))
            it->second(pWS, oProtocol);
    }

    return true;
}

bool ccIoTDeviceManager::DoRegisterCommand(std::shared_ptr<ccWebsocket> pWS, ccIoTDeviceProtocol& oProtocol)
{
    if (oProtocol._IsRequest == false)
        return false;

    std::shared_ptr<ccIoTDeviceAgent> pNewDeviceAgent(new ccIoTDeviceAgent(pWS));

    std::lock_guard<std::mutex> lock(_mtx);

    auto it = _aAgents.find(pWS->GetInstance());

    if (it != std::end(_aAgents))
        return false;

    _aAgents[pWS->GetInstance()] = pNewDeviceAgent;

    //  상태 가져오기
    ccIoTDeviceProtocol oResponseProtocol;
    oResponseProtocol.Send(pWS.get(), true, "GetDeviceStatus");

    return true;
}

bool ccIoTDeviceManager::DoDeRegisterCommand(std::shared_ptr<ccWebsocket> pWS, ccIoTDeviceProtocol& oProtocol)
{
    if (oProtocol._IsRequest == false)
        return false;

    std::lock_guard<std::mutex> lock(_mtx);

    auto it = _aAgents.find(pWS->GetInstance());

    if (it == std::end(_aAgents))
        return false;

    _aAgents.erase(it);

    return false;
}

bool ccIoTDeviceManager::DoGetDeviceStatusCommand(std::shared_ptr<ccWebsocket> pWS, ccIoTDeviceProtocol& oProtocol)
{
    if (oProtocol._IsRequest == true)
        return false;

    std::lock_guard<std::mutex> lock(_mtx);

    auto it = _aAgents.find(pWS->GetInstance());

    if (it == std::end(_aAgents))
        return false;

    return false;
}

bool ccIoTDeviceManager::DoUpdateDeviceStatusCommand(std::shared_ptr<ccWebsocket> pWS, ccIoTDeviceProtocol& oProtocol)
{
    if (oProtocol._IsRequest == false)
        return false;

    std::lock_guard<std::mutex> lock(_mtx);

    auto it = _aAgents.find(pWS->GetInstance());

    if (it == std::end(_aAgents))
        return false;

    return false;
}
