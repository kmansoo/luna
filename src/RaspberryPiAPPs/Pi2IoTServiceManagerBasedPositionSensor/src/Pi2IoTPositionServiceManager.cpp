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
            static int s_nOldValue = 0;

            if (oProtocol._oExtInfo.isNull())
                return false;

            //  If the position value  is less than 50cm, all the lights are turned on
            if (atoi(oProtocol._oExtInfo["Value"].asString().c_str()) > 50)
                this->AllLightsControl(false);
            else
                this->AllLightsControl(true);
        }
    }

    return ccIoTDeviceManager::DoUpdateDeviceStatusCommand(pWS, oProtocol);
}
