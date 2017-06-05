#ifndef __CHATTING_WS_MANAGER_H__
#define __CHATTING_WS_MANAGER_H__

#include "ccCore/ccString.h"

#include "ccWebServer/ccWebsocketManager.h"

class ChattingWSManager : public Luna::ccWebsocketManager {
public:
    ChattingWSManager();

public:
    bool ws_chat(Luna::ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<Luna::ccWebsocket> pWS, const std::string strData);
};

#endif
