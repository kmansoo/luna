#pragma once

#include <string>
#include <memory>
#include <fstream>

#include "json/value.h"
#include "restclient-cpp/connection.h"

class ConversationClient
{
public:
    ConversationClient();
    ~ConversationClient();

public:
    std::string sendText(const std::string message);

private:
    bool getEnvironment();
    bool getToken();
    bool createNewConversation();
    void addLog(bool isReq, const std::string& log);

private:
    std::string workspace_id_;
    std::string conversation_username_;
    std::string conversation_password_;

    std::string token_;
    std::string conversation_id_;
    Json::Value context_;

    std::ofstream log_file_;

    std::shared_ptr<RestClient::Connection> rest_conn_;
};

