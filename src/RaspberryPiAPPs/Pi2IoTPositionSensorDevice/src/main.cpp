
#include <iostream>

#include "ccCoreAPI/ccCoreAPI.h"
#include "ccNetworkAPI/ccNetworkManager.h"

#include "Pi2IoTPositionSensor.h"

int main(int argc, char* argv[])
{
    ccNetworkManager::getInstance().Init();

    Pi2IoTPositionSensor   oDevice;
    oDevice.Start();

    std::string         strCommand;

    while (true)
    {
        std::cout << std::endl;
        std::cout << "[Menu]" << std::endl;
        std::cout << "S: Start, T: Stop" << std::endl;
        std::cout << "Please press 'q' to stop this program. What is your command? ";

        std::cin >> strCommand;
        std::cout << std::endl;

        if (strCommand == "q")
            break;

        if (strCommand == "S")
            oDevice.Start();

        if (strCommand == "T")
            oDevice.Stop();

        Luna::sleep(10);
    }

    ccNetworkManager::getInstance().Shutdown();
}
