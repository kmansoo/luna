
#include <iostream>

#include "ccCoreAPI/ccCoreAPI.h"
#include "ccNetworkAPI/ccNetworkManager.h"
#include "ccIoTDeviceAPI/ccIoTDevice.h"

class IoTMyMultiFunctionDevice : public ccIoTDevice
{
public:
    IoTMyMultiFunctionDevice() : ccIoTDevice("MyDeviceInfo.json") {}

    virtual bool    DoSetDeviceCommand(ccIoTDeviceProtocol& oProtocol)
    {
        std::cout << "#### IoTMyMultiFunctionDevice::DoSetDeviceCommand" << std::endl;

        if (!oProtocol._oExtInfo.isNull())
        {
            if (oProtocol._oExtInfo["Control"] == "On")
                std::cout << "Control = On" << std::endl;
            else
                std::cout << "Control = Off" << std::endl;
        }

        return false;
    }

    virtual bool    DoGetDeviceStatusCommand(ccIoTDeviceProtocol& oProtocol)
    {
        std::cout << "### IoTMyMultiFunctionDevice::DoGetDeviceStatusCommand" << std::endl;

        return false;
    }
};

int main(int argc, char* argv[])
{
    ccNetworkManager::getInstance().Init();

    IoTMyMultiFunctionDevice        oMultiFunctionDevice;
    std::string                     strCommand;

    oMultiFunctionDevice.Start();

    while (true)
    {
        std::cout << "S: Start, T: Stop, A: Update Device Status" << std::endl;
        std::cout << "Please press 'q' to stop this program. What is your command? ";
        std::cin >> strCommand;
        std::cout << std::endl;

        if (strCommand == "q")
            break;

        if (strCommand == "S")
            oMultiFunctionDevice.Start();

        if (strCommand == "T")
            oMultiFunctionDevice.Stop();

        Luna::sleep(10);
    }

    ccNetworkManager::getInstance().Shutdown();
}
