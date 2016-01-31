
#include <iostream>

#include "ccCoreAPI/ccCoreAPI.h"
#include "ccNetworkAPI/ccNetworkManager.h"
#include "ccIoTDeviceAPI/ccIoTDevice.h"

class IoTSwitchDevice : public ccIoTDevice
{
public:
    IoTSwitchDevice() : ccIoTDevice("SwitchDeviceInfo.json") {}

    virtual bool    DoSetDeviceCommand(ccIoTDeviceProtocol& oProtocol)
    {
        std::cout << "#### IoTSwitchDevice::DoSetDeviceCommand" << std::endl;

        return false;
    }

    virtual bool    DoGetDeviceStatusCommand(ccIoTDeviceProtocol& oProtocol)
    {
        std::cout << "### IoTSwitchDevice::DoGetDeviceStatusCommand" << std::endl;

        return false;
    }
};

class IoTLightDevice : public ccIoTDevice
{
public:
    IoTLightDevice() : ccIoTDevice("LightDeviceInfo.json") {}

    virtual bool    DoSetDeviceCommand(ccIoTDeviceProtocol& oProtocol)
    {
        std::cout << "**** IoTLightDevice::DoSetDeviceCommand" << std::endl;

        return false;
    }

    virtual bool    DoGetDeviceStatusCommand(ccIoTDeviceProtocol& oProtocol)
    {
        std::cout << "**** IoTLightDevice::DoGetDeviceStatusCommand" << std::endl;

        return false;
    }
};

int main(int argc, char* argv[])
{
    ccNetworkManager::getInstance().Init();

    IoTSwitchDevice     oSwitchDevice;
    IoTLightDevice      oLightDevice;

    std::string         strCommand;

    oSwitchDevice.Start();
    oLightDevice.Start();

    while (true)
    {
        std::cout << "S: Start, T: Stop, A: Update Device Status" << std::endl;
        std::cout << "Please press 'q' to stop this program. What is your command? ";
        std::cin >> strCommand;
        std::cout << std::endl;

        if (strCommand == "q")
            break;

        if (strCommand == "S")
            oSwitchDevice.Start();

        if (strCommand == "T")
            oSwitchDevice.Stop();

        Luna::sleep(10);
    }

    ccNetworkManager::getInstance().Shutdown();
}
