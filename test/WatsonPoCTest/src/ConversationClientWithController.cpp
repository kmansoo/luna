#include "ConversationClientWithController.h"


#include <iostream>

#include "LogManager.h"

#include "ccCore/ccString.h"
#include "restclient-cpp/restclient.h"

#include "json/value.h"
#include "json/writer.h"
#include "json/reader.h"

ConversationClientWithController::ConversationClientWithController()
{
    // initialize RestClient
    RestClient::init();

    rest_conn_ = std::make_shared<RestClient::Connection>("https://commax-poc.mybluemix.net");

    // set connection timeout to 5s
    rest_conn_->SetTimeout(5);
    rest_conn_->SetUserAgent("Luna/IoTPlatform");
    rest_conn_->SetSSLVerifyPeer(false);

    env_.getEnvironment();

    createNewConversation();
}

ConversationClientWithController::~ConversationClientWithController()
{
}


bool ConversationClientWithController::createNewConversation() {
    RestClient::HeaderFields headers;
    headers["Accept"] = "application/json";
    headers["Content-Type"] = "application/json";

    rest_conn_->SetHeaders(headers);

    std::string req_uri = "/api/message";

    RestClient::Response response = rest_conn_->post(req_uri, "");

    if (response.code != 200) {
        std::cout << "Response:" << std::endl << response.code << std::endl << response.body << std::endl;
        return false;
    }

    Json::Reader    json_reader;
    Json::Value     new_session_info;

    LogManager::instance().addLog("CCWC", false, response.body);

    try {
        json_reader.parse(response.body, new_session_info);

        if (!new_session_info["context"].isNull()) {
            context_ = new_session_info["context"];
            conversation_id_ = context_["conversation_id"].asString();
        }
    }
    catch (...) {
        return false;
    }


    return true;
}

int ConversationClientWithController::sendText(const std::string& message, std::string& output_text, std::string& intent, std::string& body) {
    output_text = "";
    intent = "";
    body = "";

    LogManager::instance().addLog("CCWC", true, message);

    if (conversation_id_.length() == 0) {
        std::cout << "There is no Conversation ID to talk with Watson";
        return -1;
    }

    RestClient::HeaderFields headers;
    headers["Accept"] = "application/json";
    headers["Content-Type"] = "application/json";

    rest_conn_->SetHeaders(headers);

    std::string req_uri = "/api/message";
    std::string req_body;

    Json::StyledWriter writer;
    Json::Value req_json, input;

    input["text"] = message;

    req_json["context"] = context_;
    req_json["input"] = input;

    req_body = writer.write(req_json);

    //  std::cout << "req_body: " << req_body << std::endl;
    LogManager::instance().addLog("CCWC", true, req_body);

    RestClient::Response response = rest_conn_->post(req_uri, req_body);

    LogManager::instance().addLog("CCWC", false, response.body);

    if (response.code != 200) {
        std::cout << response.body << std::endl;

        return response.code;
    }

    Json::Reader    json_reader;
    Json::Value     session_info;

    try {
        body = response.body;

        json_reader.parse(response.body, session_info);

        if (!session_info["context"].isNull()) {
            context_ = session_info["context"];
            conversation_id_ = context_["conversation_id"].asString();
        }

        if (!session_info["output"].isNull()) {
            if (session_info["output"]["text"].isArray()) {
                for (int index = 0; index < session_info["output"]["text"].size(); index++) {
                    output_text += session_info["output"]["text"][index].asString();
                    output_text += "\n";
                }
            }
            else
                output_text = session_info["output"]["text"].asString();
        }

        if (!session_info["intents"].isNull()) {
            if (session_info["intents"].isArray()) {
                for (int index = 0; index < session_info["intents"].size(); index++) {
                    intent += session_info["intents"][index]["intent"].asString();
                }
            }
        }

    }
    catch (...) {
        return response.code;
    }

    return response.code;
}
