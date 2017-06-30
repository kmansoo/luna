#pragma once

#include <string>
#include <memory>

#include "EnvironmentInfo.h"

#include "json/value.h"
#include "restclient-cpp/connection.h"

// 2017.6.28, Mansoo(mansoo.kim@icloud.com), I'll implement it as soon as possible.

class TTSClient
{
public:
    TTSClient();
    ~TTSClient();

public:
    bool convert(const std::string& text);

private:
    bool getToken();

private:
    EnvironmentInfo env_;

    std::string token_;
    Json::Value context_;

    std::shared_ptr<RestClient::Connection> rest_conn_;
};

