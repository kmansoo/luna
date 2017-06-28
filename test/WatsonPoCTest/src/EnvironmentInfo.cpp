#include "EnvironmentInfo.h"


#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream> // stringstream

#include "ccCore/ccString.h"


#include "json/value.h"
#include "json/writer.h"
#include "json/reader.h"

EnvironmentInfo::EnvironmentInfo()
{
}

EnvironmentInfo::~EnvironmentInfo()
{
}

bool EnvironmentInfo::getEnvironment() {

    std::string     line;
    std::string     json_data;
    std::ifstream   env_file("env.json");

    if (env_file.is_open()) {
        while (env_file.good()) {
            std::getline(env_file, line);

            json_data += line;
        }

        env_file.close();

        Json::Reader    json_reader;
        Json::Value     environment_info;

        try {
            json_reader.parse(json_data, environment_info);

            if (!environment_info["WORKSPACE_ID"].isNull())
                this->workspace_id_ = environment_info["WORKSPACE_ID"].asCString();

            if (!environment_info["CONVERSATION_USERNAME"].isNull())
                this->conversation_username_ = environment_info["CONVERSATION_USERNAME"].asCString();

            if (!environment_info["CONVERSATION_PASSWORD"].isNull())
                this->conversation_password_ = environment_info["CONVERSATION_PASSWORD"].asCString();

            if (!environment_info["SPEECH_TO_TEXT_USERNAME"].isNull())
                this->stt_username_ = environment_info["SPEECH_TO_TEXT_USERNAME"].asCString();

            if (!environment_info["SPEECH_TO_TEXT_PASSWORD"].isNull())
                this->stt_password_ = environment_info["SPEECH_TO_TEXT_PASSWORD"].asCString();

            if (!environment_info["TEXT_TO_SPEECH_USERNAME"].isNull())
                this->tts_username_ = environment_info["TEXT_TO_SPEECH_USERNAME"].asCString();

            if (!environment_info["TEXT_TO_SPEECH_PASSWORD"].isNull())
                this->tts_password_ = environment_info["TEXT_TO_SPEECH_PASSWORD"].asCString();
        } 
        catch (...) {
            return false;
        }
    }
    else {
        std::cout << "Unable to open file: env.json";

        return false;
    }

    return true;
}
