
#include <iostream>

#include "ccCoreAPI/ccCoreAPI.h"
#include "ccNetworkAPI/ccNetworkManager.h"
#include "ccIoTDeviceManagerAPI/ccIoTDeviceManager.h"

int main(int argc, char* argv[])
{
    ccNetworkManager::getInstance().Init();

    ccIoTDeviceManager  oManager;
    std::string         strCommand;

    while (true)
    {
        std::cout << "Please press 'q' to stop this program. What is your command? ";
        std::cin >> strCommand;
        std::cout << std::endl;

        if (strCommand == "q")
            break;;

        Luna::sleep(10);
    }

    ccNetworkManager::getInstance().Shutdown();
}
