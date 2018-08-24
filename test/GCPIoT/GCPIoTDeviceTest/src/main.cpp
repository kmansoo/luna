
#include <iostream>

#include "ccCore/ccCoreAPI.h"
#include "ccNetwork/ccNetworkManager.h"

#include "ccGCPIoTDevice.h"

int main(int argc, char* argv[]) {

  if (argc < 2) {
    std::cout << "Useage: " << argv[0] << " <env.json>" << std::endl;
    return 0;
  }

  ccGCPIoTDevice device(argv[1]);

  std::string command;

  device.start();

  while (true) {
    std::cout << "S: Start, T: Stop, a, b: state" << std::endl;
    std::cout
        << "Please press 'q' to stop this program. What is your command? ";
    std::cin >> command;
    std::cout << std::endl;

    if (command == "q") break;

    std::string topic_name;
    std::string payload;

    if (command == "a") {
      topic_name = "state";
      payload = "{'power':'off'}";
    }

    if (command == "b") {
      topic_name = "state";
      payload = "{'power':'on'}";
    }


    if (payload.length() > 0) {
      device.sendMqttTopic(topic_name, payload);
      //  std::cout << "SENT: " << payload << std::endl;
    }
  
    Luna::sleep(10);
  }

  Luna::ccNetworkManager::instance().shutdown();
}
