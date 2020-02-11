
#include "ChattingWSManager.h"

ChattingWSManager::ChattingWSManager() {
    this->add_function("/ws", std::bind(&ChattingWSManager::ws_chat, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
    this->add_function("/ws_chat", std::bind(&ChattingWSManager::ws_chat, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
}


bool ChattingWSManager::ws_chat(Luna::ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<Luna::ccWebsocket> pWS, const char* data, uint32_t size, bool is_text) {
    switch ((int)eEvent) {
    case Luna::ccWebsocket::ccWebSocketEvent_Connected:
    {
        std::string strMessage;

        Luna::ccString::format(strMessage, "%08x: Join", pWS->get_instance());

        pWS->get_group()->broadcast(strMessage);
    }
    break;

    case Luna::ccWebsocket::ccWebSocketEvent_ReceivedData:
    {
        std::string strBroadcastMsg;
        std::string strMessage(data, size);

        Luna::ccString::format(strBroadcastMsg, "%08x: %s", pWS->get_instance(), strMessage.c_str());

        pWS->get_group()->broadcast(strBroadcastMsg);
    }
    break;

    case Luna::ccWebsocket::ccWebSocketEvent_Disconnected:
    {
        std::string strMessage;

        Luna::ccString::format(strMessage, "%08x: Leave", pWS->get_instance());

        pWS->get_group()->broadcast(strMessage);
    }
    break;
    }
    return true;
}
