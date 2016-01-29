
#include <iostream>

#include "ccCoreAPI/ccCoreAPI.h"
#include "ccNetworkAPI/ccNetworkManager.h"
#include "ccIoTDeviceAPI/ccIoTDevice.h"

int main(int argc, char* argv[])
{
    ccNetworkManager::getInstance().Init();

    ccIoTDevice         oDevice;
    std::string         strCommand;

    oDevice.Start("ws://localhost:8000/ws_iot_deivce");
    //oDevice.Start("ws://localhost:8126/foo");

    while (true)
    {
        std::cout << "R: Register, U: DeRegister, A: Update Device Status" << std::endl;
        std::cout << "Please press 'q' to stop this program. What is your command? ";
        std::cin >> strCommand;
        std::cout << std::endl;

        if (strCommand == "q")
            break;

        if (strCommand == "R")
        {
            ccIoTDeviceProtocol oProtocol;

            oProtocol._IsRequest = true;
            oProtocol._strCommand = "Register";

            oDevice.Send(oProtocol);
        }

        if (strCommand == "U")
        {
            ccIoTDeviceProtocol oProtocol;

            oProtocol._IsRequest = true;
            oProtocol._strCommand = "DeRegister";

            oDevice.Send(oProtocol);
        }

        Luna::sleep(10);
    }

    ccNetworkManager::getInstance().Shutdown();
}
