#include "STTClient.h"


#include <iostream>

#include "LogManager.h"

#include "ccCore/ccString.h"
#include "restclient-cpp/restclient.h"

#include "json/value.h"
#include "json/writer.h"
#include "json/reader.h"

// 2017.6.28, Mansoo(mansoo.kim@icloud.com), I'll implement it as soon as possible.

STTClient::STTClient()
{
    // initialize RestClient
    RestClient::init();

    rest_conn_ = std::make_shared<RestClient::Connection>("https://stream.watsonplatform.net");

    // set connection timeout to 5s
    rest_conn_->SetTimeout(5);
    rest_conn_->SetUserAgent("Luna/IoTPlatform");
    rest_conn_->SetSSLVerifyPeer(false);

    env_.getEnvironment();

    getToken();
}

STTClient::~STTClient()
{
}

bool STTClient::getToken() {

    if (env_.stt_username_.length() == 0 ||
        env_.stt_password_.length() == 0) {

        std::cout << "There is not enough information to get the Token.";
        return false;
    }

    std::shared_ptr<RestClient::Connection> conn = std::make_shared<RestClient::Connection>("https://stream.watsonplatform.net");

    // set connection timeout to 5s
    conn->SetTimeout(5);
    conn->SetSSLVerifyPeer(false);

    conn->SetBasicAuth(env_.stt_username_, env_.stt_password_);
    conn->SetUserAgent("Luna/IoTPlatform");

    RestClient::Response response = conn->get("/authorization/api/v1/token?url=https://stream.watsonplatform.net/speech-to-text/api");

    if (response.code != 200) {
        token_ = "";
        return false;
    }

    token_ = response.body;

    return true;
}

bool STTClient::convert(const std::string& filename) {
    RestClient::HeaderFields headers;
    headers["X-Watson-Authorization-Token"] = token_;
    headers["Accept"] = "application/json";
    headers["Content-Type"] = "audio/ogg;codecs=opus";

    rest_conn_->SetHeaders(headers);

    std::string req_uri = "/speech-to-text/api/v1/recognize";
    std::string req_body;

    std::ifstream   audio_file(filename.c_str());
/*
    if (audio_file.is_open()) { 
        while (audio_file.good()) {
            std::string line;

            std::getline(audio_file, line);

            std::cout << line << std::endl;

            req_body += line;
        }
    }
*/
    //  LogManager::instance().addLog("STTClient", true, req_body);
    RestClient::Response response = rest_conn_->post(req_uri, req_body);

    LogManager::instance().addLog("STTClient", false, response.body);

    if (response.code != 200) {
        std::cout << "Response:" << std::endl << response.code << std::endl << response.body << std::endl;
        return "";
    }

    return true;
}
