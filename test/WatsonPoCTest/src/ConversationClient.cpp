#include "ConversationClient.h"


#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream> // stringstream

#include "ccCore/ccString.h"
#include "restclient-cpp/restclient.h"

#include "json/value.h"
#include "json/writer.h"
#include "json/reader.h"

ConversationClient::ConversationClient()
{
    // initialize RestClient
    RestClient::init();

    log_file_.open("log.txt");

    rest_conn_ = std::make_shared<RestClient::Connection>("https://gateway.watsonplatform.net");

    // set connection timeout to 5s
    rest_conn_->SetTimeout(5);
    rest_conn_->SetUserAgent("Luna/IoTPlatform");
    rest_conn_->SetSSLVerifyPeer(false);

    env_.getEnvironment();

    getToken();
    createNewConversation();
}

ConversationClient::~ConversationClient()
{
}


bool ConversationClient::getToken() {

    if (env_.workspace_id_.length() == 0 ||
        env_.conversation_username_.length() == 0 ||
        env_.conversation_password_.length() == 0) {

        std::cout << "There is not enough information to get the Token.";
        return false;
    }

    std::shared_ptr<RestClient::Connection> conn = std::make_shared<RestClient::Connection>("https://gateway.watsonplatform.net");

    // set connection timeout to 5s
    conn->SetTimeout(5);
    conn->SetSSLVerifyPeer(false);

    conn->SetBasicAuth(env_.conversation_username_, env_.conversation_password_);
    conn->SetUserAgent("Luna/IoTPlatform");

    RestClient::Response response = conn->get("/authorization/api/v1/token?url=https://gateway.watsonplatform.net/conversation/api");

    if (response.code != 200) {
        token_ = "";
        return false;
    }

    token_ = response.body;

    return true;
}

bool ConversationClient::createNewConversation() {
    if (token_.length() == 0) {

        std::cout << "There is no Token to get the Conversation ID.";
        return false;
    }

    RestClient::HeaderFields headers;
    headers["X-Watson-Authorization-Token"] = token_;
    headers["Accept"] = "application/json";
    headers["Content-Type"] = "application/json";

    rest_conn_->SetHeaders(headers);

    std::string req_uri = "/conversation/api/v1/workspaces/" + env_.workspace_id_ + "/message?version=2017-05-26";

    RestClient::Response response = rest_conn_->post(req_uri, "");

    if (response.code != 200) {
        std::cout << "Response:" << std::endl << response.code << std::endl << response.body << std::endl;
        return false;
    }

    Json::Reader    json_reader;
    Json::Value     new_session_info;

    addLog(false, response.body);

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

int ConversationClient::sendText(const std::string& message, std::string& output_text, std::string& intent, std::string& body) {

    output_text = "";
    intent = "";
    body = "";

    addLog(true, message);

    if (conversation_id_.length() == 0) {
        std::cout << "There is no Conversation ID to talk with Watson";
        return -1;
    }

    RestClient::HeaderFields headers;
    headers["X-Watson-Authorization-Token"] = token_;
    headers["Accept"] = "application/json";
    headers["Content-Type"] = "application/json";

    rest_conn_->SetHeaders(headers);

    std::string req_uri = "/conversation/api/v1/workspaces/" + env_.workspace_id_ + "/message?version=2017-05-26";
    std::string req_body;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    Json::StyledWriter writer;
    Json::Value req_json, input;
    std::stringstream ss;

    ss << std::put_time(&tm, "%Y-%m-%d %H-%M-%S");

    input["text"] = message;

    req_json["context"] = context_;
    req_json["input"] = input;

    req_body = writer.write(req_json);

    addLog(true, req_body);

    RestClient::Response response = rest_conn_->post(req_uri, req_body);

    addLog(false, response.body);

    if (response.code != 200) {
        std::cout << "Response:" << std::endl << response.code << std::endl << response.body << std::endl;
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

void ConversationClient::addLog(bool isReq, const std::string& log) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    if (isReq)
        log_file_ << "[REQ] ";
    else
        log_file_ << "[***] ";

    log_file_ << std::put_time(&tm, "%d-%m-%Y %H-%M-%S: ") << log << std::endl;
}
