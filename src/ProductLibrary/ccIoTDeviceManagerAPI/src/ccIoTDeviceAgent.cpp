/*
 * ccIoTDeviceAgent.cpp
 *
 *  Created on: 2016. 1. 28.
 *      Author: kmansoo
 */

#include "ccIoTDeviceAgent.h"

ccIoTDeviceAgent::ccIoTDeviceAgent(std::shared_ptr<ccWebsocket> pWS)
{
    _pWS     = pWS;

}

ccIoTDeviceAgent::~ccIoTDeviceAgent()
{
    // TODO Auto-generated destructor stub
}

