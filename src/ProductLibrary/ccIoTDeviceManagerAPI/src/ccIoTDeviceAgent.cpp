/*
 * ccIoTDeviceAgent.cpp
 *
 *  Created on: 2016. 1. 28.
 *      Author: kmansoo
 */

#include <iostream>

#include "ccIoTDeviceAgent.h"

ccIoTDeviceAgent::ccIoTDeviceAgent(std::shared_ptr<ccWebsocket> pWS, ccIoTDeviceSpecificationList&& oSpecInfo)
{
    _pWS = pWS;

    _oDeviceSpecInfo = std::move(oSpecInfo);
}

ccIoTDeviceAgent::~ccIoTDeviceAgent()
{
    // TODO Auto-generated destructor stub
}

bool ccIoTDeviceAgent::HasDevice(ccIoTDeviceSpecification::IoTDeviceType eDeviceType)
{
    return _oDeviceSpecInfo.HasDevice(eDeviceType);
}

bool ccIoTDeviceAgent::HasDevice(const std::string& strDeviceName)
{
    return _oDeviceSpecInfo.HasDevice(strDeviceName);
}

int ccIoTDeviceAgent::GetDeviceIndex(ccIoTDeviceSpecification::IoTDeviceType eDeviceType)
{
    return _oDeviceSpecInfo.GetDeviceIndex(eDeviceType);
}

ccIoTDeviceSpecification& ccIoTDeviceAgent::GetDeviceSpec(std::size_t nIndex) 
{ 
    if (nIndex >= _oDeviceSpecInfo.Size())
        return _oNullObj;

    return _oDeviceSpecInfo.GetAt(nIndex); 
}

ccIoTDeviceSpecification::IoTDeviceType ccIoTDeviceAgent::GetType(std::size_t nIndex)
{ 
    if (nIndex >= _oDeviceSpecInfo.Size())
        return ccIoTDeviceSpecification::kDeviceType_Unknown;

    return _oDeviceSpecInfo.GetAt(nIndex).GetType(); 
}

const std::string& ccIoTDeviceAgent::GetTypeName(std::size_t nIndex)
{ 
    if (nIndex >= _oDeviceSpecInfo.Size())
        return _oNullData;

    return _oDeviceSpecInfo.GetAt(nIndex).GetTypeName(); 
}

const std::string& ccIoTDeviceAgent::GetName(std::size_t nIndex)
{ 
    if (nIndex >= _oDeviceSpecInfo.Size())
        return _oNullData;

    return _oDeviceSpecInfo.GetAt(nIndex).GetName(); 
}

void ccIoTDeviceAgent::Show()
{
    std::cout << "* ID               : " << _pWS->GetInstance() << std::endl;

    for (std::size_t nIndex = 0; nIndex < _oDeviceSpecInfo.Size(); nIndex++)
    {
        std::cout << "  ** Functon       : " << nIndex + 1 << std::endl;
        std::cout << "       Type        : " << _oDeviceSpecInfo.GetAt(nIndex).GetTypeName() << std::endl;
        std::cout << "       Name        : " << _oDeviceSpecInfo.GetAt(nIndex).GetName() << std::endl;
        std::cout << "       Description : " << _oDeviceSpecInfo.GetAt(nIndex).GetDescription() << std::endl;
        std::cout << "       Manufacture : " << _oDeviceSpecInfo.GetAt(nIndex).GetManufacture() << std::endl;
    }
}
