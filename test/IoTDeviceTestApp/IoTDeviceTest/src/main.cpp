
#include <iostream>

#include "ccCore/ccCoreAPI.h"
#include "ccNetwork/ccNetworkManager.h"
#include "ccIoTDevice/ccIoTDevice.h"

class IoTSwitchDevice : public ccIoTDevice {
public:
    IoTSwitchDevice() : ccIoTDevice("SwitchDeviceInfo.json") {}

    virtual bool    DoSetDeviceCommand(ccIoTDeviceProtocol& oProtocol) {
        std::cout << "#### IoTSwitchDevice::DoSetDeviceCommand" << std::endl;

        if (oProtocol.ext_info_.isNull())
            return false;

        if (has_device(oProtocol.ext_info_["DeviceType"].asString())) {
            if (oProtocol.ext_info_["Control"] == "On")
                std::cout << "Control = On" << std::endl;
            else
                std::cout << "Control = Off" << std::endl;
        }

        return false;
    }

    virtual bool    DoGetDeviceStatusCommand(ccIoTDeviceProtocol& oProtocol) {
        std::cout << "### IoTSwitchDevice::DoGetDeviceStatusCommand" << std::endl;

        return false;
    }
};

class IoTLightDevice : public ccIoTDevice {
public:
    IoTLightDevice() : ccIoTDevice("LightDeviceInfo.json") {}

    virtual bool    DoSetDeviceCommand(ccIoTDeviceProtocol& oProtocol) {
        std::cout << "**** IoTLightDevice::DoSetDeviceCommand" << std::endl;

        if (oProtocol.ext_info_.isNull())
            return false;

        if (ccIoTDevice::has_device(oProtocol.ext_info_["DeviceType"].asString()) == false)
            return false;

        if (oProtocol.ext_info_["Control"] == "On")
            std::cout << "Control = On" << std::endl;
        else
            std::cout << "Control = Off" << std::endl;

        return true;
    }

    virtual bool    DoGetDeviceStatusCommand(ccIoTDeviceProtocol& oProtocol) {
        std::cout << "**** IoTLightDevice::DoGetDeviceStatusCommand" << std::endl;

        return false;
    }
};

int main(int argc, char* argv[]) {
    Luna::ccNetworkManager::instance().init();

    IoTSwitchDevice     switch_device;
    IoTLightDevice      light_device;

    std::string         command;

    switch_device.start();
    light_device.start();

    while (true) {
        std::cout << "S: Start, T: Stop, A: Update Device Status" << std::endl;
        std::cout << "Please press 'q' to stop this program. What is your command? ";
        std::cin >> command;
        std::cout << std::endl;

        if (command == "q")
            break;

        if (command == "S")
            switch_device.start();

        if (command == "T")
            switch_device.stop();

        Luna::sleep(10);
    }

    Luna::ccNetworkManager::instance().shutdown();
}
