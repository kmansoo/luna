
#include <iostream>

#include "ccCore/ccCoreAPI.h"
#include "ccNetwork/ccNetworkManager.h"

#include "ConversationClient.h"
#include "TTSClient.h"

void testConversation() {
    ConversationClient  client;
    std::string         text;
    std::string         output_text, intent, body;

    std::cout << "Start conversation with Watson" << std::endl << std::endl;

    while (true) {
        std::cin.clear();
        std::cout << "You> ";
        std::getline(std::cin, text);

        if (text.size() > 0) {
            if (text == "q")
                break;

            client.sendText(text, output_text, intent, body);

            std::cout << "Watson> " << output_text << "(" << intent << ")" << std::endl;

            Luna::sleep(10);
        }

        std::cout << std::endl;

        text = "";
    }
}

void testTextToSpeech() {
    TTSClient  client;
    std::string         text;

    std::cout << "Start Text to Speech using Watson" << std::endl << std::endl;

    while (true) {
        std::cin.clear();
        std::cout << "You> ";
        std::getline(std::cin, text);

        if (text.size() > 0) {
            if (text == "q")
                break;

            std::cout << "Watson> " << client.convert(text) << std::endl;

            Luna::sleep(10);
        }

        std::cout << std::endl;

        text = "";
    }
}

int main(int argc, char* argv[]) {

    Luna::ccNetworkManager::instance().init();

    testConversation();

    return 0;
}
