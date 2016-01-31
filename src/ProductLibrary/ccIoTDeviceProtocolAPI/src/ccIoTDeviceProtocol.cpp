/*
 * ccIoTDeviceProtocol.cpp
 *
 *  Created on: 2016. 1. 29.
 *      Author: kmansoo
 */

#include "ccJsonParserAPI/json/value.h"
#include "ccJsonParserAPI/json/writer.h"
#include "ccJsonParserAPI/json/reader.h"

#include "ccIoTDeviceProtocol.h"


ccIoTDeviceProtocol::ccIoTDeviceProtocol()
{
    // TODO Auto-generated constructor stub

    _aParsers["Register"]           = std::bind(&ccIoTDeviceProtocol::DoParserRegisterCommand, this, std::placeholders::_1);
    _aParsers["DeRegister"]         = std::bind(&ccIoTDeviceProtocol::DoParserDeRegisterCommand, this, std::placeholders::_1);
    _aParsers["SetDevice"]          = std::bind(&ccIoTDeviceProtocol::DoParserSetDeviceCommand, this, std::placeholders::_1);
    _aParsers["GetDeviceStatus"]    = std::bind(&ccIoTDeviceProtocol::DoParserGetDeviceStatusCommand, this, std::placeholders::_1);
}

ccIoTDeviceProtocol::~ccIoTDeviceProtocol()
{
    // TODO Auto-generated destructor stub
}

bool ccIoTDeviceProtocol::Send(ccWebsocket* oWS)
{
    return Send(oWS, _IsRequest, _strCommand, _oExtInfo);
}

bool ccIoTDeviceProtocol::Send(ccWebsocket* oWS, bool bIsRequest, const std::string& strCommand)
{
    Json::Value oNoInfo;

    return Send(oWS, bIsRequest, strCommand, oNoInfo);
}

bool ccIoTDeviceProtocol::Send(ccWebsocket* oWS, bool bIsRequest, const std::string& strCommand, const Json::Value& oExtInfo)
{
    std::string strData;

    Json::Value oProtocol;
    Json::StyledWriter oWriter;

    oProtocol["Request"] = bIsRequest;
    oProtocol["Command"] = strCommand;

    if (!oExtInfo.isNull())
        oProtocol["Info"] = oExtInfo;

    strData = std::move(oWriter.write(oProtocol));

    return oWS->Send(strData);
}

bool ccIoTDeviceProtocol::Parse(const std::string& strData)
{
    Json::Reader    oReader;
    Json::Value     oProtocol;

    if (!oReader.parse(strData, oProtocol))
        return false;

    if (oProtocol.isObject() == false)
        return false;

    if (oProtocol["Request"].isNull())
        return false;

    if (oProtocol["Command"].isNull())
        return false;

    _IsRequest  = oProtocol["Request"].asBool();
    _strCommand = std::move(oProtocol["Command"].asString());

    if (oProtocol["Info"].isObject() == true)
        _oExtInfo = std::move(oProtocol["Info"]);

    return true;
}

//  I'll implement the following method.
bool ccIoTDeviceProtocol::DoParserRegisterCommand(const Json::Value& oExtInfo)
{
    
    return false;
}

bool ccIoTDeviceProtocol::DoParserDeRegisterCommand(const Json::Value& oExtInfo)
{

    return false;
}

bool ccIoTDeviceProtocol::DoParserSetDeviceCommand(const Json::Value& oExtInfo)
{

    return false;
}

bool ccIoTDeviceProtocol::DoParserGetDeviceStatusCommand(const Json::Value& oExtInfo)
{

    return false;
}
