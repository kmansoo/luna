/*
 * Pi2IoTPositionServiceManager.cpp
 *
 *  Created on: 2016. 2. 4.
 *      Author: kmansoo
 */

#include "Pi2IoTPositionServiceManager.h"

Pi2IoTPositionServiceManager::Pi2IoTPositionServiceManager()
{
    // TODO Auto-generated constructor stub

}

Pi2IoTPositionServiceManager::~Pi2IoTPositionServiceManager()
{
    // TODO Auto-generated destructor stub
}

bool Pi2IoTPositionServiceManager::DoUpdateDeviceStatusCommand(std::shared_ptr<ccWebsocket> pWS, ccIoTDeviceProtocol& oProtocol)
{
    std::shared_ptr<ccIoTDeviceAgent> pDA;

    if (DoFindeDeviceAgent(pWS, pDA))
    {
        if (pDA->GetType() == ccIoTDeviceSpecification::kDeviceType_Sensor)
        {
            if (oProtocol._oExtInfo.isNull())
                return false;

            std::cout << "## Pi2IoTPositionServiceManager, Position : " << atoi(oProtocol._oExtInfo["Value"].asString().c_str()) << " cm" << std::endl;
        }
    }

    return ccIoTDeviceManager::DoUpdateDeviceStatusCommand(pWS, oProtocol);
}
