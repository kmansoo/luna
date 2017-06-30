#include "TTSClient.h"


#include <iostream>

#include "LogManager.h"

#include "ccCore/ccString.h"
#include "restclient-cpp/restclient.h"

#include "json/value.h"
#include "json/writer.h"
#include "json/reader.h"

// 2017.6.28, Mansoo(mansoo.kim@icloud.com), I'll implement it as soon as possible.

TTSClient::TTSClient()
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

TTSClient::~TTSClient()
{
}

bool TTSClient::getToken() {

    if (env_.tts_username_.length() == 0 ||
        env_.tts_password_.length() == 0) {

        std::cout << "There is not enough information to get the Token.";
        return false;
    }

    std::shared_ptr<RestClient::Connection> conn = std::make_shared<RestClient::Connection>("https://stream.watsonplatform.net");

    // set connection timeout to 5s
    conn->SetTimeout(5);
    conn->SetSSLVerifyPeer(false);

    conn->SetBasicAuth(env_.tts_username_, env_.tts_password_);
    conn->SetUserAgent("Luna/IoTPlatform");

    RestClient::Response response = conn->get("/authorization/api/v1/token?url=https://stream.watsonplatform.net/text-to-speech/api");

    if (response.code != 200) {
        token_ = "";
        return false;
    }

    token_ = response.body;

    return true;
}


bool TTSClient::convert(const std::string& text) {
    RestClient::HeaderFields headers;
    headers["X-Watson-Authorization-Token"] = token_;
    headers["Accept"] = "audio/ogg;codecs=opus";
    headers["Content-Type"] = "application/json";

    rest_conn_->SetHeaders(headers);

    std::string req_uri = "/text-to-speech/api/v1/synthesize";
    std::string req_body;

    Json::StyledWriter writer;
    Json::Value req_json;

    req_json["text"] = text;

    req_body = writer.write(req_json);

    LogManager::instance().addLog("TTSClient", true, req_body);

    RestClient::Response response = rest_conn_->post(req_uri, req_body);

    if (response.code != 200) {
        std::cout << "Response:" << std::endl << response.code << std::endl << response.body << std::endl;
        return "";
    }

    std::ofstream wav_file;

    wav_file.open("tts.opus", std::ios::out | std::ios::binary);
    wav_file.write(response.body.c_str(), response.body.length());

    return true;
}
