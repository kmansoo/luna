
#include "ccCore/ccString.h"
#include "ccNetwork/ccNetworkManager.h"

#include "restclient-cpp/restclient.h"
#include "restclient-cpp/connection.h"

#include "json/value.h"
#include "json/writer.h"
#include "json/reader.h"

int main(int argc, char* argv[]) {

    Luna::ccNetworkManager::instance().init();

    // initialize RestClient
    RestClient::init();

    /*
    RestClient::Connection* conn = new RestClient::Connection("http://220.120.109.11:4500");

    // set connection timeout to 5s
    conn->SetTimeout(5);

    // set custom user agent
    conn->SetUserAgent("Luna/RestClient");

    // set headers
    RestClient::HeaderFields headers;

    headers["Accept"] = "application/json";
    headers["Content-Type"] = "application/json";
    conn->SetHeaders(headers);

    Json::Value request, resource, user;

    Json::Reader reader;
    Json::Value  result;
    Json::StyledWriter writer;

    //RestClient::Response exist_response = conn->get(checkIDAPI); // 테스트 서버
    //RestClient::Response post = conn->post("/cmx/register", writer.write(request)); // 테스트 서버
    */
    return 0;
}