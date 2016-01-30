/*
 * ccIoTDeviceAgent.cpp
 *
 *  Created on: 2016. 1. 28.
 *      Author: kmansoo
 */

#include <iostream>

#include "ccIoTDeviceAgent.h"

ccIoTDeviceAgent::ccIoTDeviceAgent(std::shared_ptr<ccWebsocket> pWS, const Json::Value& oSpec)
{
    _pWS = pWS;

    _oSpecification.Parse(oSpec);
}

ccIoTDeviceAgent::~ccIoTDeviceAgent()
{
    // TODO Auto-generated destructor stub
}

void ccIoTDeviceAgent::Show()
{
    std::cout << "* ID          : " << _pWS->GetInstance() << std::endl;
    std::cout << "* Type        : " << _oSpecification.GetTypeName() << std::endl;
    std::cout << "* Name        : " << _oSpecification.GetName() << std::endl;
    std::cout << "* Description : " << _oSpecification.GetDescription() << std::endl;
    std::cout << "* Manufacture : " << _oSpecification.GetManufacture() << std::endl;
}
