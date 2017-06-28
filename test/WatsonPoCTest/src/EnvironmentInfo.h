#pragma once

#include <string>
#include <memory>
#include <fstream>

#include "json/value.h"

class EnvironmentInfo
{
public:
    EnvironmentInfo();
    ~EnvironmentInfo();

public:
    bool getEnvironment();

public:
    std::string workspace_id_;
    std::string conversation_username_;
    std::string conversation_password_;

    std::string stt_username_;
    std::string stt_password_;
    
    std::string tts_username_;
    std::string tts_password_;
};

