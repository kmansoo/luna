/*
 * ccIoTDeviceSpecification.cpp
 *
 *  Created on: 2016. 1. 29.
 *      Author: kmansoo
 */

#include <fstream>

#include "ccJsonParserAPI/json/reader.h"

#include "ccIoTDeviceSpecification.h"

std::string s_aDeviceTypeName[]{
    "Unknown",
    "Switch",
    "Light",
    "Lock",
    "Sensor"
};

ccIoTDeviceSpecification::ccIoTDeviceSpecification()
{
    _eDeviceType = kDeviceType_Unknown;
}

ccIoTDeviceSpecification::~ccIoTDeviceSpecification()
{
    // TODO Auto-generated destructor stub
}

const std::string& ccIoTDeviceSpecification::GetTypeName()
{
    if ((std::uint16_t)_eDeviceType > kDeviceType_Sensor)
        return s_aDeviceTypeName[0];

    return s_aDeviceTypeName[_eDeviceType];
}

bool ccIoTDeviceSpecification::LoadFile(const std::string& strFileName)
{
    std::ifstream ifs(strFileName);

    if (!ifs)
        return false;

    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    return Parse(content);
}

bool ccIoTDeviceSpecification::Parse(const Json::Value& oInfo)
{
    if (oInfo.isObject() == false)
        return false;

    if (oInfo["IoTDeviceSpecification"].isNull())
        return false;

    if (oInfo["IoTDeviceMasterUri"].isNull())
        return false;

    if (oInfo["IoTDeviceSpecification"]["Type"].isNull())
        return false;

    if (oInfo["IoTDeviceSpecification"]["Name"].isNull())
        return false;

    _oSpecificationValue = oInfo;

    std::string strTemp =_oSpecificationValue["IoTDeviceSpecification"]["Type"].asString();

    _eDeviceType = kDeviceType_Unknown;

    for (int nIndex = 0; nIndex < kDeviceType_Max; nIndex++)
    {
        if (strTemp == s_aDeviceTypeName[nIndex])
            _eDeviceType = (IoTDeviceType)nIndex;
    }

    _strName = _oSpecificationValue["IoTDeviceSpecification"]["Name"].asCString();

    if (!_oSpecificationValue["IoTDeviceSpecification"]["Description"].isNull())
        _strDescription = _oSpecificationValue["IoTDeviceSpecification"]["Description"].asString();

    if (!_oSpecificationValue["IoTDeviceSpecification"]["Manufacture"].isNull())
        _strManufacture = _oSpecificationValue["IoTDeviceSpecification"]["Manufacture"].asString();

    _strIoTDeviceMasterUri = std::move(oInfo["IoTDeviceMasterUri"].asString());

    return true;
}

bool ccIoTDeviceSpecification::Parse(const std::string& strInfo)
{
    Json::Reader    oReader;
    Json::Value     oRootValue;

    if (!oReader.parse(strInfo, oRootValue))
        return false;

    return Parse(oRootValue);
}
