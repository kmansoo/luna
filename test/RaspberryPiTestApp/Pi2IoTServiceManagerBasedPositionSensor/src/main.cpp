
#include <iostream>

#include "ccCore/ccCoreAPI.h"
#include "ccNetwork/ccNetworkManager.h"

#include "Pi2IoTPositionServiceManager.h"

int main(int argc, char* argv[])
{
    ccNetworkManager::getInstance().Init();

    Pi2IoTPositionServiceManager    oManager;
    std::string                     strCommand;

    while (true)
    {
        std::cout << "[Menu]" << std::endl;
        std::cout << "1: Show all IoT Device info" << std::endl;
        std::cout << "2: All Switches On" << std::endl;
        std::cout << "3: All Switches Off" << std::endl;
        std::cout << "4: ALl Lights On" << std::endl;
        std::cout << "5: ALl Lights Off" << std::endl;
        std::cout << "6: ALl Locks On" << std::endl;
        std::cout << "7: ALl Locks Off" << std::endl;
        std::cout << "q: Quit" << std::endl;
        std::cout << std::endl;
        std::cout << "What is your command? ";
        std::cin >> strCommand;
        std::cout << std::endl;

        if (strCommand == "q")
            break;

        switch (atoi(strCommand.c_str()))
        {
        case 1: oManager.Show(); break;
        case 2: oManager.AllSwitchesControl(true); break;
        case 3: oManager.AllSwitchesControl(false); break;
        case 4: oManager.AllLightsControl(true); break;
        case 5: oManager.AllLightsControl(false); break;
        case 6: oManager.AllLocksControl(true); break;
        case 7: oManager.AllLocksControl(false); break;
        }

        Luna::sleep(10);
    }

    ccNetworkManager::getInstance().Shutdown();
}
