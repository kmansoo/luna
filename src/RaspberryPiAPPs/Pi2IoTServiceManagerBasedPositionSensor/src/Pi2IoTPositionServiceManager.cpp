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

        //  It was implemented to show what linked to iOS App.
        //  It delivers the received protocol to the Controller.
        std::lock_guard<std::mutex> lock(_mtx);

        for (auto it : _aAgents)
        {
            if (it.second->GetType() == ccIoTDeviceSpecification::kDeviceType_Controller)
                oProtocol.Send(it.second->GetWebsockt().get());
        }
    }

    return ccIoTDeviceManager::DoUpdateDeviceStatusCommand(pWS, oProtocol);
}

bool Pi2IoTPositionServiceManager::DoSetControlCommand(std::shared_ptr<ccWebsocket> pWS, ccIoTDeviceProtocol& oProtocol)
{
    //  We need to implement this feature.
    if (oProtocol._IsRequest == false)
        return false;

    _mtx.lock();
    auto it = _aAgents.find(pWS->GetInstance());
    _mtx.unlock();

    if (it == std::end(_aAgents))
        return false;

    if (it->second->GetType() == ccIoTDeviceSpecification::kDeviceType_Controller)
    {
        if (oProtocol._oExtInfo["Control"] == "AllLightsTurnOff")
            AllLightsControl(false);

        if (oProtocol._oExtInfo["Control"] == "AllLightsTurnOn")
            AllLightsControl(true);
    }

    return true;
}
