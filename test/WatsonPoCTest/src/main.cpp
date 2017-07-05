
#include <iostream>

#include "ccIoTDevice/ccIoTDeviceManager.h"

#include "ccCore/ccCoreAPI.h"
#include "ccCore/ccString.h"
#include "ccNetwork/ccNetworkManager.h"

#include "ConversationClient.h"
#include "ConversationClientWithController.h"

#include "STTClient.h"
#include "TTSClient.h"

#include "AudioManager/AudioManager.h"

void testConversationWithController() {
    ccIoTDeviceManager  manager;

    ConversationClientWithController client;
    AudioManager audio_manager(-1, -1);

    TTSClient tts_client;
    STTClient stt_client;
    std::string text;
    std::string output_text, intent, body;

    std::string current_latitude = "37.3247140";
    std::string current_longitude = "127.1073318";

    std::cout << "Start conversation with Controller talks " << std::endl
        << std::endl;

    enum IntentStatus {
        kIntentStatus_NORMAL = 0,
        kIntentStatus_SERACH_DEVICE,
        kIntentStatus_REGISTER_DEVICE,
        kIntentStatus_OUT_OF_HOME,
        kIntentStatus_OUT_OF_HOME_DETECTION,
    };

    enum CommaxActionStatus {
        kCommaxAction_READY = 0,
        kCommaxAction_SEARCH_DEVICE,
        kCommaxAction_REGISTER_DEVICE_START,
        kCommaxAction_GET_DEVICE_INSTALL_INFO,
        kCommaxAction_REGISTER_DEVICE_RUN,
        kCommaxAction_CHECK_DEVICE_COUNT,
        kCommaxAction_REGISTER_DEVICE_END,

        kCommaxAction_GET_ROOM_STATUS,
        kCommaxAction_SET_ROOM_STATUS,
        kCommaxAction_GET_HOME_STATUS,
        kCommaxAction_CHECK_USER_COMMAND,
        kCommaxAction_SET_HOME_STATUS,
        kCommaxAction_SET_STATUS_END,

        kCommaxAction_GET_WEATHER_INFO,
        kCommaxAction_CHECK_RAIN_CONDITION,
        kCommaxAction_CHECK_UV_CONDITION,
        kCommaxAction_MAKE_ADVICE,
        kCommaxAction_MAKE_ADVICE_END,

        kCommaxAction_MAX
    };

    std::string commax_action_list[]{
      "xxREADYxx",
      "SEARCH_DEVICE",
      "REGISTER_DEVICE_START",
      "GET_DEVICE_INSTALL_INFO",
      "REGISTER_DEVICE_RUN",
      "CHECK_DEVICE_COUNT",
      "REGISTER_DEVICE_END",
      "GET_ROOM_STATUS",
      "SET_ROOM_STATUS",
      "GET_HOME_STATUS",
      "CHECK_USER_COMMAND",
      "SET_HOME_STATUS",
      "SET_STATUS_END",
      "GET_WEATHER_INFO",
      "CHECK_RAIN_CONDITION",
      "CHECK_UV_CONDITION",
      "MAKE_ADVICE",
      "MAKE_ADVICE_END"
    };

    IntentStatus intent_status = kIntentStatus_NORMAL;
    CommaxActionStatus commax_action_status = kCommaxAction_READY;

    while (true) {
        std::cin.clear();
        std::cout << "Usage: '/': record your voice, '*': detect an event on camera, '!': set latitude and longitude" << std::endl;
        std::cout << "You> ";

        std::getline(std::cin, text);

        if (text.size() > 0) {
            if (text == "/") {
                std::cout << "{I am listening for 4 seconds to record your voice.!}"
                    << std::endl;

                audio_manager.record("stt.opus", 4);
                audio_manager.play("stt.opus");

                std::vector<std::string> converted_text_list;
                stt_client.convert("stt.opus", converted_text_list);

                if (converted_text_list.size() > 0)
                    text = converted_text_list[0];
            }

            if (text == "q")
                break;

            if (text == "!") {
                std::cin.clear();
                std::cout << std::endl;
                std::cout << "BOT> Latitude: ";
                std::getline(std::cin, text);

                if (text == "")
                    text = "34.96";

                current_latitude = text;

                std::cout << "BOT> Longitude: ";
                std::getline(std::cin, text);

                if (text == "")
                    text = "127.7277803";

                current_longitude = text;
                std::cout << std::endl;
                continue;
            }

            if (text == "*") {
                client.getContext()["gooutsignal"] = 1;
                client.getContext()["latitude"] = current_latitude;
                client.getContext()["longitude"] = current_longitude;

                client.getContext()["objectarray"].clear();

                std::cin.clear();
                std::cout << std::endl;

                std::cout << "BOT> Do you have a umbrella? (0 = no, 1 = yes) ";
                std::getline(std::cin, text);

                if (text == "1")
                    client.getContext()["objectarray"][client.getContext()["objectarray"].size()] = "umbrella";

                std::cout << "BOT> Did you wear sunglasses?? (0 = no, 1 = yes) ";
                std::getline(std::cin, text);

                if (text == "1")
                    client.getContext()["objectarray"][client.getContext()["objectarray"].size()] = "sunglasses";

                client.getContext()["umbrella"] = 0;

                text = "weather";
            }

        send_text:
            int rep_code = client.sendText(text, output_text, intent, body);

            std::cout << "Watson> " << output_text << "<Intent: " << intent
                << "/ commax_action: "
                << client.getContext()["commaxaction"].asString() << ">"
                << std::endl;

            if (output_text.length() > 0) {
                tts_client.convert(output_text);

                audio_manager.play("tts.opus");
            }

            if (rep_code != 200) {
                continue;
            }

            IntentStatus current_intent_status = kIntentStatus_NORMAL;
            CommaxActionStatus current_commax_action_status = kCommaxAction_READY;

            //  for intent
            if (intent == "SERACH_DEVICE")
                current_intent_status = kIntentStatus_SERACH_DEVICE;

            if (intent == "REGISTER_DEVICE")
                current_intent_status = kIntentStatus_REGISTER_DEVICE;

            if (intent == "OUT_OF_HOME")
                current_intent_status = kIntentStatus_OUT_OF_HOME;

            //  for commaxaction
            for (int index = 0; index < kCommaxAction_MAX; index++) {
                if (client.getContext()["commaxaction"].asString() == commax_action_list[index]) {
                    current_commax_action_status = (CommaxActionStatus)index;
                    break;
                }
            }

            // change states
            if (current_intent_status != kIntentStatus_NORMAL) {
                intent_status = current_intent_status;
                commax_action_status = kCommaxAction_READY;
            }

            // if (current_commax_action_status != kCommaxAction_NORMAL)
            commax_action_status = current_commax_action_status;

            switch ((int)intent_status) {
            case kIntentStatus_SERACH_DEVICE:
            case kIntentStatus_REGISTER_DEVICE:
                switch ((int)commax_action_status) {
                case kCommaxAction_SEARCH_DEVICE:
                {
                    std::cout << "BOT> I'm waiting for about 10 seconds for IoT devices to be registered." << std::endl;

                    for (int count = 0; count < 10; count++) {
                        std::cout << (count + 1)  << ".";
                        Luna::sleep(1 * 1000);
                    }

                    std::cout << std::endl;

                    /*
                    std::cin.clear();
                    std::cout << std::endl;
                    std::cout << "BOT> How many devices did you find? ";
                    std::getline(std::cin, text);
                    std::cout << std::endl;
                    */
                    intent_status = kIntentStatus_REGISTER_DEVICE;

                    client.getContext()["newdevicecount"] = (int)manager.get_device_count();
                    text = "";
                    goto send_text;
                }
                break;

                case kCommaxAction_REGISTER_DEVICE_START:
                    manager.sendControlCommand(0, "blink_start");
                    break;

                case kCommaxAction_REGISTER_DEVICE_RUN:
                    manager.sendControlCommand(0, "blink_stop");

                    client.getContext()["validation"] = 1;
                    text = "";
                    goto send_text;
                    break;

                case kCommaxAction_REGISTER_DEVICE_END:
                    intent_status = kIntentStatus_NORMAL;
                    break;
                }
                break;

            case kIntentStatus_OUT_OF_HOME:
                switch ((int)commax_action_status) {
                case kCommaxAction_GET_ROOM_STATUS:
                {
                    std::cin.clear();
                    std::cout << std::endl;
                    std::cout << "BOT> Are there any turned lights in the room?(0 = no, 1 = yes) ";
                    std::getline(std::cin, text);
                    std::cout << std::endl;

                    Json::Value device_status;

                    device_status[0] = "home01";
                    device_status[1] = "my home";
                    device_status[2] = "zone01";
                    device_status[3] = "my zone";
                    device_status[4] = "room01";
                    device_status[5] = "my room";
                    device_status[6] = "device01";
                    device_status[7] = "my light";

                    if (text == "0")
                        device_status[8] = "off";
                    else
                        device_status[8] = "on";

                    client.getContext()["devicefullarray"][0] = device_status;

                    text = "";
                    goto send_text;
                }
                break;

                case kCommaxAction_SET_ROOM_STATUS:
                    client.getContext()["validation"] = 1;
                    text = "";
                    goto send_text;
                    break;

                case kCommaxAction_GET_HOME_STATUS:
                {
                    std::cin.clear();
                    std::cout << std::endl;
                    std::cout << "BOT> Are there any turned lights in home?(0 = no, 1 = yes) ";
                    std::getline(std::cin, text);
                    std::cout << std::endl;

                    Json::Value device_status;

                    device_status[0] = "home01";
                    device_status[1] = "my home";
                    device_status[2] = "zone01";
                    device_status[3] = "my zone";
                    device_status[4] = "room01";
                    device_status[5] = "my room";
                    device_status[6] = "device01";
                    device_status[7] = "my light";
                    device_status[8] = "off";
                    client.getContext()["devicefullarray"][0] = device_status;

                    // device2 in room2
                    device_status[4] = "room02";
                    device_status[5] = "my bedroom";
                    device_status[6] = "device02";
                    device_status[7] = "my light";

                    if (text == "0")
                        device_status[8] = "off";
                    else
                        device_status[8] = "on";

                    client.getContext()["devicefullarray"][1] = device_status;

                    text = "";
                    goto send_text;
                }
                break;

                case kCommaxAction_CHECK_USER_COMMAND:
                    break;

                case kCommaxAction_SET_HOME_STATUS:
                    client.getContext()["validation"] = 1;
                    text = "";
                    goto send_text;
                    break;

                case kCommaxAction_SET_STATUS_END:
                    break;
                }
                break;
            }

            Luna::sleep(10);
        }

        std::cout << std::endl;

        text = "";
    }
}

void testTextToSpeech() {
    TTSClient client;
    std::string text;

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

int main(int argc, char *argv[]) {

    Luna::ccNetworkManager::instance().init();

    testConversationWithController();
    // testTextToSpeech();

    return 0;
}
