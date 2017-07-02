#include "STTClient.h"

#include <stdio.h>

#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>

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
    rest_conn_->SetTimeout(30);
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

bool STTClient::convert(const std::string& filename, std::vector<std::string>& converted_text_list) {
    converted_text_list.clear();

    RestClient::HeaderFields headers;
    headers["X-Watson-Authorization-Token"] = token_;
    headers["Accept"] = "application/json";
    headers["Content-Type"] = "audio/ogg;codecs=opus";

    rest_conn_->SetHeaders(headers);

    std::vector<unsigned char> binary_buffer;

    std::ifstream in_file(filename, std::ios_base::binary | std::ios_base::in);
    std::istream_iterator<unsigned char> end_of_file;
    std::istream_iterator<unsigned char> in_file_iter(in_file);

    FILE* fp = fopen(filename.c_str(), "rb");

    while (!feof(fp))
        binary_buffer.push_back(fgetc(fp));

    fclose(fp);

    /*
        while (in_file_iter != end_of_file) {
            binary_buffer.push_back(*in_file_iter++);

            read_count++;
        }
    */

    std::string req_uri = "/speech-to-text/api/v1/recognize?model=en-US_BroadbandModel&inactivity_timeout=30&max_alternatives=1&word_confidence=false&timestamps=false&profanity_filter=true&smart_formatting=false&speaker_labels=false";
    std::string req_body(binary_buffer.begin(), binary_buffer.end());

    //  LogManager::instance().addLog("STTClient", true, req_body);
    RestClient::Response response = rest_conn_->post(req_uri, req_body);

    LogManager::instance().addLog("STTClient", false, response.body);

    std::cout << "Response:" << std::endl << response.code << std::endl << response.body << std::endl;

    if (response.code != 200) {
        std::cout << "Response:" << std::endl << response.code << std::endl << response.body << std::endl;
        return "";
    }

    try {
        Json::Reader    json_reader;
        Json::Value     converted_result_info;

        json_reader.parse(response.body, converted_result_info);

        if (!converted_result_info["results"].isNull()) {
            for (int index = 0; index < converted_result_info["results"].size(); index++) {
                converted_text_list.push_back(converted_result_info["results"][index]["alternatives"][0]["transcript"].asString());
            }
        }
    }
    catch (...) {
        return false;
    }    

    return true;
}
