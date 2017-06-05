
#include <iostream>

#include "ccCore/ccCoreAPI.h"
#include "ccNetwork/ccNetworkManager.h"
#include "ccIoTDevice/ccIoTDeviceManager.h"

int main(int argc, char* argv[]) {
    Luna::ccNetworkManager::instance().init();

    ccIoTDeviceManager  manager;
    std::string         command;

    while (true) {
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
        std::cin >> command;
        std::cout << std::endl;

        if (command == "q")
            break;

        switch (atoi(command.c_str())) {
        case 1: manager.show(); break;
        case 2: manager.control_all_switches(true); break;
        case 3: manager.control_all_switches(false); break;
        case 4: manager.control_all_lights(true); break;
        case 5: manager.control_all_lights(false); break;
        case 6: manager.control_all_locks(true); break;
        case 7: manager.control_all_locks(false); break;
        }

        Luna::sleep(10);
    }

    Luna::ccNetworkManager::instance().shutdown();
}
