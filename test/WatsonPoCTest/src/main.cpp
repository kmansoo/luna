
#include <iostream>

#include "ccCore/ccCoreAPI.h"
#include "ccCore/ccString.h"
#include "ccNetwork/ccNetworkManager.h"

#include "ConversationClient.h"
#include "TTSClient.h"

void testConversation() {
    ConversationClient  client;
    TTSClient           tts_client;
    std::string         text;
    std::string         output_text, intent, body;

    std::cout << "Start conversation with Watson" << std::endl << std::endl;

    enum IntentStatus {
        kIntentStatus_NORMAL = 0,
        kIntentStatus_SERACH_DEVICE,
        kIntentStatus_REGISTER_DEVICE,
        kIntentStatus_OUT_OF_HOME,
    };

    enum CommaxActionStatus {
        kCommaxAction_NORMAL = 0,
        kCommaxAction_NEWSEARCH,
        kCommaxAction_STARTREGISTER,
        kCommaxAction_NEWREGISTER,
        kCommaxAction_ENDREGISTER,
    };

    IntentStatus intent_status = kIntentStatus_NORMAL;
    CommaxActionStatus commax_action_status = kCommaxAction_NORMAL;

    while (true) {
        std::cin.clear();
        std::cout << "You> ";
        std::getline(std::cin, text);

        if (text.size() > 0) {
            if (text == "q")
                break;

send_text:
            client.sendText(text, output_text, intent, body);
            
            tts_client.convert(output_text);

            std::cout << "Watson> " << output_text << "(" << intent << ")" << std::endl;
            system("cvlc --play-and-exit tts.ogg");

            IntentStatus current_intent_status = kIntentStatus_NORMAL;
            CommaxActionStatus current_commax_action_status = kCommaxAction_NORMAL;

            //  for intent
            if (intent == "SERACH_DEVICE")
                current_intent_status = kIntentStatus_SERACH_DEVICE;

            if (intent == "REGISTER_DEVICE")
                current_intent_status = kIntentStatus_REGISTER_DEVICE;

            if (intent == "OUT_OF_HOME")
                current_intent_status = kIntentStatus_OUT_OF_HOME;

            //  for commaxaction
            if (client.getContext()["commaxaction"].asString() == "NEWSEARCH")
                current_commax_action_status = kCommaxAction_NEWSEARCH;

            if (client.getContext()["commaxaction"].asString() == "STARTREGISTER")
                current_commax_action_status = kCommaxAction_STARTREGISTER;

            if (client.getContext()["commaxaction"].asString() == "NEWREGISTER")
                current_commax_action_status = kCommaxAction_NEWREGISTER;

            if (client.getContext()["commaxaction"].asString() == "ENDREGISTER")
                current_commax_action_status = kCommaxAction_ENDREGISTER;

            // change states
            if (current_intent_status != kIntentStatus_NORMAL) {
                intent_status = current_intent_status;
                commax_action_status = kCommaxAction_NORMAL;
            }

            //if (current_commax_action_status != kCommaxAction_NORMAL)
                commax_action_status = current_commax_action_status;

            switch (intent_status) {
            case kIntentStatus_NORMAL:
                break;

            case kIntentStatus_SERACH_DEVICE:
                switch (commax_action_status) {
                case kCommaxAction_NEWSEARCH:
                    std::cin.clear();
                    std::cout << std::endl;
                    std::cout << "BOT> How many devices did you find? ";
                    std::getline(std::cin, text);
                    std::cout << std::endl;

                    intent_status = kIntentStatus_REGISTER_DEVICE;

                    client.getContext()["newdevicecount"] = atoi(text.c_str());
                    text = "";
                    goto send_text;
                    break;
                break;
                }

            case kIntentStatus_REGISTER_DEVICE:
                switch (commax_action_status) {
                case kCommaxAction_ENDREGISTER:
                    intent_status = kIntentStatus_NORMAL;
                    break;

                case kCommaxAction_NORMAL:
                case kCommaxAction_STARTREGISTER:
                case kCommaxAction_NEWREGISTER:
                {
                    std::vector<std::string> infos;

                    while (true) {
                        std::cin.clear();
                        std::cout << std::endl;
                        std::cout << "BOT> Where do you want to install and what is the device name (place, name) ? ";
                        std::getline(std::cin, text);
                        std::cout << std::endl;

                        infos = Luna::ccString::splitToken(text, ',');

                        if (infos.size() > 0)
                            break;
                    }

                    client.getContext()["newdeviceplace"] = infos[0];

                    if (infos.size() >= 2)
                        client.getContext()["newdevicename"] = infos[1];

                    client.getContext()["validation"] = 1;

                    text = "install";
                    client.sendText(text, output_text, intent, body);
                    goto send_text;
                    break;
                }
                break;
                }

            case kIntentStatus_OUT_OF_HOME:
                break;
            }

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
    // testTextToSpeech();

    return 0;
}
