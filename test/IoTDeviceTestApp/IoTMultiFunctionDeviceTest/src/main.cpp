
#include <iostream>

#include "ccCore/ccCoreAPI.h"
#include "ccNetwork/ccNetworkManager.h"
#include "ccIoTDevice/ccIoTDevice.h"

class IoTMyMultiFunctionDevice : public ccIoTDevice {
public:
    IoTMyMultiFunctionDevice() : ccIoTDevice("MyDeviceInfo.json") {}

    virtual bool    DoSetDeviceCommand(ccIoTDeviceProtocol& oProtocol) {
        std::cout << "#### IoTMyMultiFunctionDevice::DoSetDeviceCommand" << std::endl;

        if (!oProtocol.ext_info_.isNull()) {
            if (oProtocol.ext_info_["Control"] == "On")
                std::cout << "Control = On" << std::endl;
            else
                std::cout << "Control = Off" << std::endl;
        }

        return false;
    }

    virtual bool    DoGetDeviceStatusCommand(ccIoTDeviceProtocol& oProtocol) {
        std::cout << "### IoTMyMultiFunctionDevice::DoGetDeviceStatusCommand" << std::endl;

        return false;
    }
};

int main(int argc, char* argv[]) {
    Luna::ccNetworkManager::instance().init();

    IoTMyMultiFunctionDevice        oMultiFunctionDevice;
    std::string                     strCommand;

    oMultiFunctionDevice.start();

    while (true) {
        std::cout << "S: Start, T: Stop, A: Update Device Status" << std::endl;
        std::cout << "Please press 'q' to stop this program. What is your command? ";
        std::cin >> strCommand;
        std::cout << std::endl;

        if (strCommand == "q")
            break;

        if (strCommand == "S")
            oMultiFunctionDevice.start();

        if (strCommand == "T")
            oMultiFunctionDevice.stop();

        Luna::sleep(10);
    }

    Luna::ccNetworkManager::instance().shutdown();
}
