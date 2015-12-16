
#include <memory>
#include <thread>

#include "ccCoreAPI/ccString.h"
#include "ccWebServerAPI/ccWebServerManager.h"

#include "ccMongooseServerAPI/ccMongooseWebServerObjectFactory.h"
#include "ccRESTfulChattingAPI/ccRESTfulChatting.h"

#include "ccWebServerAPI/ccWebsocketManager.h"

class ccChattingWSManager : public ccWebsocketManager
{
public:
    ccChattingWSManager()
    {
        this->AddFunction("/ws", std::bind(&ccChattingWSManager::ws_chat, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
        this->AddFunction("/ws_chat", std::bind(&ccChattingWSManager::ws_chat, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    }

public:
    bool ws_chat(ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<ccWebsocket> pWS, const char* strData, std::size_t size)
    {
        switch ((int)eEvent)
        {
        case ccWebsocket::ccWebSocketEvent_Connected:
            {
                std::string strMessage;

                ccString::format(strMessage, "%08x: Join", pWS->GetInstance());

                pWS->GetGroup()->Broadcast(strMessage);
            }
            break;

        case ccWebsocket::ccWebSocketEvent_Data:
            {
                std::string strBroadcastMsg;
                std::string strMessage(strData, size);

                ccString::format(strBroadcastMsg, "%08x: %s", pWS->GetInstance(), strMessage.c_str());

                pWS->GetGroup()->Broadcast(strBroadcastMsg);
            }
            break;

        case ccWebsocket::ccWebSocketEvent_Close:
            {
                std::string strMessage;

                ccString::format(strMessage, "%08x: Leave", pWS->GetInstance());

                pWS->GetGroup()->Broadcast(strMessage);
            }
            break;
        }
        return true;
    }

};

//  #include "ccBoostServerAPI/ccBoostWebServerObjectFactory.h"

int main(int argc, char* argv[])
{
    ccWebServerManager  oManager;

    //  choose a Web Server : Abstract Factory Design Pattern

    oManager.AttachFactory(std::make_shared<ccMongooseWebServerObjectFactory>());
    //oManager.AttachFactory(std::make_shared<ccBoostWebServerObjectFactory>());

    std::shared_ptr<ccRESTfulChatting>      pChattingApi(new ccRESTfulChatting);
    std::shared_ptr<ccWebsocketManager>     pChattingWSM(new ccChattingWSManager);

    oManager.CreateWebServer("WebServer #1", "8000");
    oManager.AddRESTfulApi(pChattingApi);
    oManager.AddWebsocketManager(pChattingWSM);

    oManager.Start();

    while (1)
        std::this_thread::sleep_for(std::chrono::microseconds{ 100 });
}
