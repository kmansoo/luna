
#include <iostream>

#include "ccCore/ccCoreAPI.h"
#include "ccNetwork/ccNetworkManager.h"
#include "ccGCPIoTDevice/ccGCPIoTDevice.h"

int main(int argc, char* argv[]) {
  ccGCPIoTDevice device;

  std::string command;

  device.start();

  while (true) {
    std::cout << "S: Start, T: Stop, A: Update Device Status" << std::endl;
    std::cout
        << "Please press 'q' to stop this program. What is your command? ";
    std::cin >> command;
    std::cout << std::endl;

    if (command == "q") break;

    Luna::sleep(10);
  }

  Luna::ccNetworkManager::instance().shutdown();
}
