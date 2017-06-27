
#include <iostream>

#include "ccCore/ccCoreAPI.h"
#include "ccNetwork/ccNetworkManager.h"

#include "ConversationClient.h"

int main(int argc, char* argv[]) {

    Luna::ccNetworkManager::instance().init();

    ConversationClient  client;
    std::string         text;

    std::cout << "Start conversation with Watson" << std::endl << std::endl;

    while (true) {
        std::cin.clear();
        std::cout << "You> ";
        std::getline(std::cin, text);

        if (text.size() > 0) {
            if (text == "q")
                break;

            std::cout << "Watson> " << client.sendText(text) << std::endl;

            Luna::sleep(10);
        }

        std::cout << std::endl;

        text = "";
    }

    return 0;
}
