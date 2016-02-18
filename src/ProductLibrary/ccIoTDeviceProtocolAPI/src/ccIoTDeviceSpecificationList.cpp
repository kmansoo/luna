/*
 * ccIoTDeviceSpecificationList.cpp
 *
 *  Created on: 2016. 1. 29.
 *      Author: kmansoo
 */

#include <fstream>

#include "ccJsonParserAPI/json/reader.h"

#include "ccIoTDeviceSpecificationList.h"

ccIoTDeviceSpecificationList::ccIoTDeviceSpecificationList()
{
}

ccIoTDeviceSpecificationList::ccIoTDeviceSpecificationList(const ccIoTDeviceSpecificationList& other)
{
    this->_oSpecificationValue  = other._oSpecificationValue;
    //this->_aSpecificationList.resize(other._aSpecificationList.size());
    
    memcpy(&this->_aSpecificationList.at(0), &other._aSpecificationList.at(0), other._aSpecificationList.size());
}


ccIoTDeviceSpecificationList::ccIoTDeviceSpecificationList(ccIoTDeviceSpecificationList&& other)
{
    *this = std::move(other);
}

ccIoTDeviceSpecificationList::~ccIoTDeviceSpecificationList()
{
    // TODO Auto-generated destructor stub
}

ccIoTDeviceSpecificationList& ccIoTDeviceSpecificationList::operator=(ccIoTDeviceSpecificationList&& other) 
{
    if (this == &other)
        return *this;

    this->_oSpecificationValue = std::move(other._oSpecificationValue);
    this->_aSpecificationList = std::move(other._aSpecificationList);

    return *this;
}

bool ccIoTDeviceSpecificationList::LoadFile(const std::string& strFileName)
{
    std::ifstream ifs(strFileName);

    if (!ifs)
        return false;

    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    return Parse(content);
}

bool ccIoTDeviceSpecificationList::Parse(const Json::Value& oInfo)
{
    if (oInfo.isNull())
        return false;

    _oSpecificationValue["IoTDeviceSpecification"] = oInfo;

    if (oInfo.isArray() == false)
    {
        ccIoTDeviceSpecification    oSpecification;

        if (oSpecification.Parse(oInfo))
            _aSpecificationList.push_back(std::move(oSpecification));
    }
    else
    {
        for (Json::ArrayIndex nIndex = 0; nIndex < oInfo.size(); nIndex++)
        {
            ccIoTDeviceSpecification    oSpecification;

            if (oSpecification.Parse(oInfo[nIndex]) == true)
                _aSpecificationList.push_back(std::move(oSpecification));
        }
    }

    return true;
}

bool ccIoTDeviceSpecificationList::Parse(const std::string& strInfo)
{
    Json::Reader    oReader;
    Json::Value     oRootValue;

    if (!oReader.parse(strInfo, oRootValue))
        return false;

    return Parse(oRootValue);
}

//  public method
std::size_t ccIoTDeviceSpecificationList::Size() 
{
    return _aSpecificationList.size();
}

std::vector<ccIoTDeviceSpecification>&  ccIoTDeviceSpecificationList::GetList() 
{
    return _aSpecificationList;
}

bool ccIoTDeviceSpecificationList::HasDevice(ccIoTDeviceSpecification::IoTDeviceType eDeviceType)
{
    for (auto it : _aSpecificationList)
    {
        if (it.GetType() == eDeviceType)
            return true;
    }

    return false;
}

bool ccIoTDeviceSpecificationList::HasDevice(const std::string& strDeviceName)
{
    return HasDevice(ccIoTDeviceSpecification::getType(strDeviceName));
}

int ccIoTDeviceSpecificationList::GetDeviceIndex(ccIoTDeviceSpecification::IoTDeviceType eDevice)
{
    for (std::size_t nIndex = 0; nIndex < _aSpecificationList.size(); nIndex++)
    {
        if (_aSpecificationList[nIndex].GetType() == eDevice)
            return nIndex;
    }

    return -1;
}

ccIoTDeviceSpecification& ccIoTDeviceSpecificationList::GetAt(std::size_t nIndex) 
{
    if (nIndex >= _aSpecificationList.size())
        return _oNullObj;

    return _aSpecificationList[nIndex];
}

const Json::Value& ccIoTDeviceSpecificationList::ToJson()
{
    return _oSpecificationValue;
}
