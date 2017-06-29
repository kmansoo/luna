#include "STTClient.h"


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

// 2017.6.28, Mansoo(mansoo.kim@icloud.com), I'll implement it as soon as possible.

STTClient::STTClient()
{
    // initialize RestClient
    RestClient::init();

    log_file_.open("log.txt");

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


void STTClient::addLog(bool isReq, const std::string& log) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    if (isReq)
        log_file_ << "[REQ] ";
    else
        log_file_ << "[***] ";

    log_file_ << std::put_time(&tm, "%d-%m-%Y %H-%M-%S: ") << log << std::endl;
}
