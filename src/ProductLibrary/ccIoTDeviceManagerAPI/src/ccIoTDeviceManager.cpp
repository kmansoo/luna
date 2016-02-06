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
    _aCommands["SetControl"] = std::bind(&ccIoTDeviceManager::DoSetControlCommand, this, std::placeholders::_1, std::placeholders::_2);

    // TODO Auto-generated constructor stub
    _oWebAPI->AddAPI(std::string("/devices"), std::bind(&ccIoTDeviceManager::devices, this, std::placeholders::_1, std::placeholders::_2));
    _oWebAPI->AddAPI(std::string("/devices/device"), std::bind(&ccIoTDeviceManager::devices_device, this, std::placeholders::_1, std::placeholders::_2));

    _oWSM->AddFunction("/ws_iot_deivce", std::bind(&ccIoTDeviceManager::ws_IoTDevice, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    ccWebServerManager::getInstance().AttachFactory(std::make_shared<ccMongooseWebServerObjectFactory>());

#ifdef	WIN32
    ccWebServerManager::getInstance().CreateWebServer("WebServer #1", "8000", "C:\\web_root\\Chatting\\");
#else
    ccWebServerManager::getInstance().CreateWebServer("WebServer #1", "8000", ".");
#endif

    ccWebServerManager::getInstance().AddRESTfulApi(_oWebAPI);
    ccWebServerManager::getInstance().AddWebsocketManager(_oWSM);

    ccWebServerManager::getInstance().Start();
}

ccIoTDeviceManager::~ccIoTDeviceManager()
{
    // TODO Auto-generated destructor stub
}

void ccIoTDeviceManager::Show()
{
    std::lock_guard<std::mutex> lock(_mtx);

    int nIndex = 0;
    for (auto it : _aAgents)
    {
        std::cout << "-----------------------------------------------" << std::endl;
        std::cout << "* Index: " << (nIndex++ + 1) << std::endl;

        it.second->Show();

        std::cout << "-----------------------------------------------" << std::endl;
    }
}

void ccIoTDeviceManager::AllSwitchesControl(bool bOn)
{
    std::lock_guard<std::mutex> lock(_mtx);

    ccIoTDeviceProtocol oControlProtocol;

    Json::Value oExtInfo;

    if (bOn)
        oExtInfo["Control"] = "On";
    else
        oExtInfo["Control"] = "Off";

    for (auto it : _aAgents)
    {
        if (it.second->GetType() == ccIoTDeviceSpecification::kDeviceType_Switch)
            oControlProtocol.Send(it.second->GetWebsockt().get(), true, "SetDevice", oExtInfo);
    }
}

void ccIoTDeviceManager::AllLightsControl(bool bOn)
{
    std::lock_guard<std::mutex> lock(_mtx);

    ccIoTDeviceProtocol oControlProtocol;

    Json::Value oExtInfo;

    if (bOn)
        oExtInfo["Control"] = "On";
    else
        oExtInfo["Control"] = "Off";

    for (auto it : _aAgents)
    {
        if (it.second->GetType() == ccIoTDeviceSpecification::kDeviceType_Light)
            oControlProtocol.Send(it.second->GetWebsockt().get(), true, "SetDevice", oExtInfo);
    }
}

void ccIoTDeviceManager::AllLocksControl(bool bOpen)
{
    std::lock_guard<std::mutex> lock(_mtx);

    ccIoTDeviceProtocol oControlProtocol;

    Json::Value oExtInfo;

    if (bOpen)
        oExtInfo["Control"] = "Open";
    else
        oExtInfo["Control"] = "Close";

    for (auto it : _aAgents)
    {
        if (it.second->GetType() == ccIoTDeviceSpecification::kDeviceType_Lock)
            oControlProtocol.Send(it.second->GetWebsockt().get(), true, "SetDevice", oExtInfo);
    }
}

//  Open API
bool ccIoTDeviceManager::devices(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)
{
    //std::cout << "enter ccRESTfulChatting::list, query_string=" << pRequest->GetQueryString() << endl;

    switch (pRequest->GetMethod())
    {
    case ccWebServerRequest::HttpMethod_Post:   //  Create
        pResponse->Status(406, std::string("Not Acceptable"));
        pResponse->Write("");
        break;

    case ccWebServerRequest::HttpMethod_Get:    //  Read
        {
            pResponse->Status(200, std::string("OK"));

            Json::Value oResponseJsonData;
            Json::Value oDeviceList;
            Json::StyledWriter oWriter;

            oDeviceList["count"] = (int)_aAgents.size();

            int nIndex = 0;

            for (auto it : _aAgents)
            { 
                Json::Value oDeviceInfo;

                oDeviceInfo = it.second->GetDeviceSPec().ToJson();
                oDeviceInfo["ID"] = it.second->GetID();

                oDeviceList["device"][nIndex++] = oDeviceInfo;
            }

            oResponseJsonData["device_list"] = oDeviceList;

            std::string strJsonData = oWriter.write(oResponseJsonData);

            pResponse->ContentType("application/javascript", strJsonData.length());
            pResponse->Write(strJsonData);

            return true;
        }
        break;

    case ccWebServerRequest::HttpMethod_Delete:   //  Delete
        pResponse->Status(406, std::string("Not Acceptable"));
        pResponse->Write("");
        break;

    default:
        break;
    }

    return false;
}

bool ccIoTDeviceManager::devices_device(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)
{
    //std::cout << "enter ccRESTfulChatting::list, query_string=" << pRequest->GetQueryString() << endl;

    switch (pRequest->GetMethod())
    {
    case ccWebServerRequest::HttpMethod_Post:   //  Create
        pResponse->Status(406, std::string("Not Acceptable"));
        pResponse->Write("");
        break;

    case ccWebServerRequest::HttpMethod_Get:    //  Read
    {
        std::string strID = std::move(pRequest->GetVar("ID"));

        int nDeviceID = std::stoi(strID);

        auto it = _aAgents.find(nDeviceID);

        if (it == std::end(_aAgents))
        {
            pResponse->Status(404, std::string("Not Found"));
            return false;
        }

        pResponse->Status(200, std::string("OK"));

        Json::Value oResponseJsonData;
        Json::StyledWriter oWriter;

        Json::Value oDeviceInfo;

        oDeviceInfo = it->second->GetDeviceSPec().ToJson();
        oDeviceInfo["ID"] = it->second->GetID();

        oResponseJsonData["device"] = oDeviceInfo;

        std::string strJsonData = oWriter.write(oResponseJsonData);

        pResponse->ContentType("application/javascript", strJsonData.length());
        pResponse->Write(strJsonData);

        return true;
    }
    break;

    case ccWebServerRequest::HttpMethod_Put:    //  Control
    {
        int nDeviceID = std::stoi(pRequest->GetVar("ID"));

        auto it = _aAgents.find(nDeviceID);

        if (it == std::end(_aAgents))
        {
            pResponse->Status(404, std::string("Not Found"));
            return false;
        }

        pResponse->Status(200, std::string("OK"));

        std::string strControlData;

        pRequest->GetContentBody(strControlData);

        ccIoTDeviceProtocol oControlProtocol;
        oControlProtocol.Parse(strControlData);
        oControlProtocol.Send(it->second->GetWebsockt().get());

        pResponse->Write("");
        return true;
    }
    break;

    case ccWebServerRequest::HttpMethod_Delete:   //  Delete
        pResponse->Status(406, std::string("Not Acceptable"));
        pResponse->Write("");
        break;

    default:
        break;
    }

    return false;
}


//  WebSocket
bool ccIoTDeviceManager::ws_IoTDevice(ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<ccWebsocket> pWS, const std::string strData)
{
    switch ((int)eEvent)
    {
    case ccWebsocket::ccWebSocketEvent_Connected:
        //  std::cout << "ccIoTDeviceManager: An IoT device is connected. { " << std::endl << strData << std::endl << "}" << std::endl;
        break;

    case ccWebsocket::ccWebSocketEvent_ReceivedData:
        //  std::cout << "ccIoTDeviceManager: We got the ommmand from IoT Device. { " << std::endl << strData << std::endl << "}" << std::endl;
        DoParseCommand(pWS, strData);
        break;

    case ccWebsocket::ccWebSocketEvent_Disconnected:
        //  std::cout << "ccIoTDeviceManager: An IoT device is disconnected." << std::endl;
        {
            ccIoTDeviceProtocol oNoProtocol;

            oNoProtocol._IsRequest = true;

            DoDeRegisterCommand(pWS, oNoProtocol);
        }
        break;
    }

    return true;
}

bool ccIoTDeviceManager::DoFindeDeviceAgent(std::shared_ptr<ccWebsocket> pWS, std::shared_ptr<ccIoTDeviceAgent>& pDA)
{
    auto it = _aAgents.find(pWS->GetInstance());

    if (it == std::end(_aAgents))
        return false;

    pDA = it->second;

    return true;
}

bool  ccIoTDeviceManager::DoParseCommand(std::shared_ptr<ccWebsocket> pWS, const std::string strData)
{
    ccIoTDeviceProtocol oProtocol;
    if (oProtocol.Parse(strData))
    {
        //if (oProtocol._IsRequest)
        //    std::cout << "ccIoTDevice Request Command: " << oProtocol._strCommand << std::endl;
        //else
        //    std::cout << "ccIoTDevice Response Command: " << oProtocol._strCommand << std::endl;
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

    std::shared_ptr<ccIoTDeviceAgent> pNewDeviceAgent(new ccIoTDeviceAgent(pWS, oProtocol._oExtInfo));

    std::lock_guard<std::mutex> lock(_mtx);

    auto it = _aAgents.find(pWS->GetInstance());

    if (it != std::end(_aAgents))
        return false;

    _aAgents[pWS->GetInstance()] = pNewDeviceAgent;

    std::cout << "Registerd: " << pNewDeviceAgent->GetTypeName() << ", " << pNewDeviceAgent->GetName() << std::endl;

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

    std::cout << "DeRegisterd: " << it->second->GetTypeName() << ", " << it->second->GetName() << std::endl;

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

bool ccIoTDeviceManager::DoSetControlCommand(std::shared_ptr<ccWebsocket> pWS, ccIoTDeviceProtocol& oProtocol)
{
    //  We need to implement this feature.

    return false;
}
