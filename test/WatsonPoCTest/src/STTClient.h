#pragma once

#include <string>
#include <memory>
#include <fstream>

#include "EnvironmentInfo.h"

#include "json/value.h"
#include "restclient-cpp/connection.h"

// 2017.6.28, Mansoo(mansoo.kim@icloud.com), I'll implement it as soon as possible.

class STTClient
{
public:
    STTClient();
    ~STTClient();

public:

private:
    bool getToken();
    void addLog(bool isReq, const std::string& log);

private:
    EnvironmentInfo env_;

    std::string token_;
    std::string conversation_id_;
    Json::Value context_;

    std::ofstream log_file_;

    std::shared_ptr<RestClient::Connection> rest_conn_;
};

