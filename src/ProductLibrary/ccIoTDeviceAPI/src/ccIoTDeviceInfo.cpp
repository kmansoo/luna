/*
 * ccIoTDeviceInfo.cpp
 *
 *  Created on: 2016. 1. 29.
 *      Author: kmansoo
 */

#include <fstream>

#include "ccJsonParserAPI/json/reader.h"

#include "ccIoTDeviceInfo.h"

ccIoTDeviceInfo::ccIoTDeviceInfo()
{
}

ccIoTDeviceInfo::ccIoTDeviceInfo(const ccIoTDeviceInfo& other) :
    _aSpecificationInfo(other._aSpecificationInfo),
    _strIoTDeviceMasterUri(other._strIoTDeviceMasterUri)
{
}

ccIoTDeviceInfo::ccIoTDeviceInfo(ccIoTDeviceInfo&& other)
{
    *this = std::move(other);
}

ccIoTDeviceInfo::~ccIoTDeviceInfo()
{
    // TODO Auto-generated destructor stub
}

ccIoTDeviceInfo& ccIoTDeviceInfo::operator=(ccIoTDeviceInfo&& other)
{
    if (this == &other)
        return *this;

    _aSpecificationInfo = std::move(other._aSpecificationInfo);
    _strIoTDeviceMasterUri  = std::move(other._strIoTDeviceMasterUri);

    return *this;
}


bool ccIoTDeviceInfo::LoadFile(const std::string& strFileName)
{
    std::ifstream ifs(strFileName);

    if (!ifs)
        return false;

    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    return Parse(content);
}

bool ccIoTDeviceInfo::Parse(const Json::Value& oInfo)
{
    if (oInfo.isObject() == false)
        return false;

    if (oInfo["IoTDeviceSpecification"].isNull())
        return false;

    if (oInfo["IoTDeviceMasterUri"].isNull())
        return false;

    _aSpecificationInfo.Parse(oInfo["IoTDeviceSpecification"]);
    _strIoTDeviceMasterUri = oInfo["IoTDeviceMasterUri"].asString();

    return true;
}

bool ccIoTDeviceInfo::Parse(const std::string& strInfo)
{
    Json::Reader    oReader;
    Json::Value     oRootValue;

    if (!oReader.parse(strInfo, oRootValue))
        return false;

    return Parse(oRootValue);
}
