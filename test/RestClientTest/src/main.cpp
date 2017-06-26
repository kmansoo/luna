
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

    //  Now, I'm going to test with COMMAX Wallpad Emulator
    //
    RestClient::Connection* conn = new RestClient::Connection("http://localhost:8000");

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

    RestClient::Response response = conn->get("/v1/accessory/lightbulb");

    reader.parse(response.body, result);

    std::string control =
        R"(
            {
              "power_status": false,
              "bright_level": 100
            }
        )";   
    
    RestClient::Response post = conn->put("/v1/accessory/lightbulb/lightbulb1", control);

    return 0;
}
