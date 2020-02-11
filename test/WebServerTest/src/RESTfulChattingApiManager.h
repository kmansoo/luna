/*
 * ccRESTfulChattingApi.h
 *
 *  Created on: 2015. 11. 28.
 *      Author: kmansoo
 */

#ifndef CCPRODUCTLIBRARY_CCRESTFULCHATTINGAPI_CCRESTFULCHATTING_H_
#define CCPRODUCTLIBRARY_CCRESTFULCHATTINGAPI_CCRESTFULCHATTING_H_

#include <vector>

#include "ccWebServer/ccRESTfulApi.h"
#include "ccWebServer/ccWebsocketManager.h"

class RESTfulChattingUserInfo {
public:
    std::string id_;
    std::string name_;
};

class RESTfulChattingMessageInfo {
public:
    std::string date_time_;
    std::string from_id_;
    std::string message_;
};

class RESTfulChattingSessionInfo {
public:
    RESTfulChattingSessionInfo(const std::string& session_name, const std::string& owner_id);
    ~RESTfulChattingSessionInfo() {}

public:
    bool    join(const std::string& user_id);
    bool    leave(const std::string& user_id);
    bool    getMemberList(std::vector<std::string>& user_id_list);

    bool    newMessage(const std::string& user_id, const std::string& message);

public:
    std::string     name_;
    std::string     owner_id;

private:
    std::vector<std::string>    member_list_;
    std::vector<std::shared_ptr<RESTfulChattingMessageInfo>>  message_info_list_;
};

class RESTfulChattingSessionManager {
public:
    RESTfulChattingSessionManager() {}

public:
    bool    create_id(const std::string& id, const std::string& name);
    bool    delete_id(const std::string& id);
    bool    get_user_info(const std::string& id, RESTfulChattingUserInfo& user_info);
    bool    get_user_list(std::vector<std::string>& user_id_list);

    bool    create_session(const std::string& name, const std::string& owner_id);
    bool    delete_session(const std::string& name, const std::string& request_id);

    bool    join_session(const std::string& session_name, const std::string& user_id);
    bool    leave_session(const std::string& session_name, const std::string& user_id);

    bool    get_session_list(std::vector<std::string>& session_name_list, std::vector<std::string>& owner_id_list);
    bool    get_session_info(const std::string& session_name, std::vector<std::string>& member_list);

    bool    broadcast_message(const std::string& session_name, const std::string& user_id, const std::string& message);

protected:
    std::shared_ptr<RESTfulChattingSessionInfo> find_session(const std::string& name);

private:
    std::vector< std::shared_ptr<RESTfulChattingUserInfo>>    user_info_list_;
    std::vector< std::shared_ptr<RESTfulChattingSessionInfo>> session_info_list_;
};

class RESTfulChattingApiManager :
    public Luna::ccRESTfulApi,
    public Luna::ccWebsocketManager {
public:
    RESTfulChattingApiManager();
    virtual ~RESTfulChattingApiManager();

protected:
    bool    user(std::shared_ptr<Luna::ccWebServerRequest> request, std::shared_ptr<Luna::ccWebServerResponse> response);
    bool    session(std::shared_ptr<Luna::ccWebServerRequest> request, std::shared_ptr<Luna::ccWebServerResponse> response);
    bool    session_member(std::shared_ptr<Luna::ccWebServerRequest> request, std::shared_ptr<Luna::ccWebServerResponse> response);
    bool    session_member_id(std::shared_ptr<Luna::ccWebServerRequest> request, std::shared_ptr<Luna::ccWebServerResponse> response);
    bool    session_message(std::shared_ptr<Luna::ccWebServerRequest> request, std::shared_ptr<Luna::ccWebServerResponse> response);

protected:
    bool    ws_chat(Luna::ccWebsocket::ccWebSocketEvent ws_event, std::shared_ptr<Luna::ccWebsocket> websocket, const char* data, uint32_t size, bool is_text);

private:
    RESTfulChattingSessionManager chat_session_manager_;
};


#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVER_H_ */
