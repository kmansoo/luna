
#include <memory>

#include "ccCore/ccCoreAPI.h"
#include "ccCore/ccString.h"

#include "ccNetwork/ccNetworkManager.h"

#include "ccWebServer/ccWebServerManager.h"

#include "ccMongooseServer/ccMongooseWebServerObjectFactory.h"
#include "ccWebServer/ccWebsocketManager.h"

#include "ccWebsocketClient/ccEasyWebsocketClient.h"

#include "ChattingWSManager.h"
#include "RESTfulChattingApiManager.h"

int main(int argc, char* argv[]) {

    Luna::ccNetworkManager::instance().init();

    //  choose a Web Server : Abstract Factory Design Pattern
    std::string html_path = ".";

    if (argc >= 2)
        html_path = argv[1];;

    Luna::ccWebServerManager::instance().attach_factory(std::make_shared<Luna::ccMongooseWebServerObjectFactory>());

    std::shared_ptr<RESTfulChattingApiManager>  restful_api_and_ws_manager = std::make_shared<RESTfulChattingApiManager>();
    std::shared_ptr<ChattingWSManager>  only_ws_chat_manager = std::make_shared<ChattingWSManager>();

    Luna::ccWebServerManager::instance().create_web_server("WebServer #1", "8000", html_path);

    Luna::ccWebServerManager::instance().add_restful_api(restful_api_and_ws_manager);

    // We are making two websocket managers. 
    // The only_ws_chat_manager that instanced of ChattingWSManager is based on mongoose server example.
    Luna::ccWebServerManager::instance().add_websocket_manager(restful_api_and_ws_manager);
    Luna::ccWebServerManager::instance().add_websocket_manager(only_ws_chat_manager);

    Luna::ccWebServerManager::instance().start();

    while (1)
        Luna::sleep(100);
}
