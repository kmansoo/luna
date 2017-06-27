
#include <iostream>

#include "ccCore/ccString.h"
#include "ccNetwork/ccNetworkManager.h"

#include "restclient-cpp/restclient.h"
#include "restclient-cpp/connection.h"


int main(int argc, char* argv[]) {

    Luna::ccNetworkManager::instance().init();

    // initialize RestClient
    RestClient::init();

    RestClient::Connection* conn = new RestClient::Connection("https://httpbin.org");

    conn->SetSSLVerifyPeer(false);

    // set connection timeout to 5s
    conn->SetTimeout(5);
    conn->SetUserAgent("Luna/RestClient");


    RestClient::Response response = conn->get("/get");
    std::cout << "Response:" << std::endl << response.code << std::endl << response.body << std::endl;

    return 0;
}
