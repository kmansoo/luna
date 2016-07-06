
#include <memory>

#include "ccCore/ccCoreAPI.h"
#include "ccCore/ccString.h"

#include "ccWebServer/ccWebServerManager.h"

#include "ccMongooseServer/ccMongooseWebServerObjectFactory.h"
#include "ccRESTfulChattingAPI/ccRESTfulChatting.h"

#include "ccWebServer/ccWebsocketManager.h"

#include "ccWebsocketClient/ccEasyWebsocketClient.h"

class ccChattingWSManager : public Luna::ccWebsocketManager
{
public:
    ccChattingWSManager()
    {
        this->addFunction("/ws", std::bind(&ccChattingWSManager::ws_chat, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        this->addFunction("/ws_chat", std::bind(&ccChattingWSManager::ws_chat, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

public:
    bool ws_chat(Luna::ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<Luna::ccWebsocket> pWS, const std::string strData)
    {
        switch ((int)eEvent)
        {
        case Luna::ccWebsocket::ccWebSocketEvent_Connected:
            {
                std::string strMessage;

                Luna::ccString::format(strMessage, "%08x: Join", pWS->getInstance());

                pWS->getGroup()->broadcast(strMessage);
            }
            break;

        case Luna::ccWebsocket::ccWebSocketEvent_ReceivedData:
            {
                std::string strBroadcastMsg;
                std::string strMessage(strData);

                Luna::ccString::format(strBroadcastMsg, "%08x: %s", pWS->getInstance(), strMessage.c_str());

                pWS->getGroup()->broadcast(strBroadcastMsg);
            }
            break;

        case Luna::ccWebsocket::ccWebSocketEvent_Disconnected:
            {
                std::string strMessage;

                Luna::ccString::format(strMessage, "%08x: Leave", pWS->getInstance());

                pWS->getGroup()->broadcast(strMessage);
            }
            break;
        }
        return true;
    }

};

#ifdef WIN32
#   include <Winsock2.h>
#endif

int main(int argc, char* argv[])
{
#ifdef WIN32
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    int err;

    err = WSAStartup(wVersionRequested, &wsaData);

    if (err != 0)
    {
        //// could not find a usable WinSock DLL
        ////cerr << "Could not load winsock" << endl;
        //assert(0); // is this is failing, try a different version that 2.2, 1.0 or later will likely work
        //exit(1);
    }
#endif

    //  choose a Web Server : Abstract Factory Design Pattern

    Luna::ccWebServerManager::getInstance().attachFactory(std::make_shared<Luna::ccMongooseWebServerObjectFactory>());
    //oManager.AttachFactory(std::make_shared<ccBoostWebServerObjectFactory>());

    std::shared_ptr<ccRESTfulChatting>      pChattingApi(new ccRESTfulChatting);
    std::shared_ptr<Luna::ccWebsocketManager>     pChattingWSM(new ccChattingWSManager);

    Luna::ccWebServerManager::getInstance().createWebServer("WebServer #1", "8000", ".");

    Luna::ccWebServerManager::getInstance().addRESTfulApi(pChattingApi);
    Luna::ccWebServerManager::getInstance().addWebsocketManager(pChattingApi);

    Luna::ccWebServerManager::getInstance().addWebsocketManager(pChattingWSM);

    Luna::ccWebServerManager::getInstance().start();

    while (1)
        Luna::sleep(100);
}
