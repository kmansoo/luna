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
    "Sensor",
    "Controller"
};

ccIoTDeviceSpecification::ccIoTDeviceSpecification()
{
    _eDeviceType = kDeviceType_Unknown;
}

ccIoTDeviceSpecification::ccIoTDeviceSpecification(const ccIoTDeviceSpecification& other)
{
    _eDeviceType            = other._eDeviceType;
    _strName                = other._strName;
    _strDescription         = other._strDescription;
}


ccIoTDeviceSpecification::ccIoTDeviceSpecification(ccIoTDeviceSpecification&& other)
{
    *this = std::move(other);
}

ccIoTDeviceSpecification::~ccIoTDeviceSpecification()
{
    // TODO Auto-generated destructor stub
}

const std::string& ccIoTDeviceSpecification::getTypeNameString(IoTDeviceType eType)
{
    if ((std::uint16_t)eType >= kDeviceType_Max)
        return s_aDeviceTypeName[0];

    return s_aDeviceTypeName[eType];
}

ccIoTDeviceSpecification::IoTDeviceType ccIoTDeviceSpecification::getType(const std::string& strName)
{
    for (int nIndex = 0; nIndex < kDeviceType_Max; nIndex++)
    {
        if (strName == s_aDeviceTypeName[nIndex])
            return (IoTDeviceType)nIndex;
    }

    return kDeviceType_Unknown;
}

ccIoTDeviceSpecification& ccIoTDeviceSpecification::operator=(ccIoTDeviceSpecification&& other) 
{
    if (this == &other)
        return *this;

    _eDeviceType    = other._eDeviceType;
    _strName        = std::move(other._strName);
    _strDescription = std::move(other._strDescription);

    return *this;
}

const std::string& ccIoTDeviceSpecification::GetTypeName()
{
    return getTypeNameString(_eDeviceType);
}

bool ccIoTDeviceSpecification::Parse(const Json::Value& oInfo)
{
    if (oInfo.isObject() == false)
        return false;

    if (oInfo["Type"].isNull())
        return false;

    if (oInfo["Name"].isNull())
        return false;

    _eDeviceType    = getType(oInfo["Type"].asString());
    _strName        = oInfo["Name"].asCString();

    if (!oInfo["Description"].isNull())
        _strDescription = oInfo["Description"].asString();

    if (!oInfo["Manufacture"].isNull())
        _strManufacture = oInfo["Manufacture"].asString();

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
