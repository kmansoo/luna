/*
 * ccRESTfulWebServer.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include <iostream>
#include <functional>

#include <iomanip>
#include <ctime>

#include <sstream>

#include "ccCore/ccString.h"

#include "json/value.h"
#include "json/writer.h"
#include "json/reader.h"

#include "RESTfulChattingApiManager.h"

RESTfulChattingSessionInfo::RESTfulChattingSessionInfo(const std::string& session_name, const std::string& owner_id) :
    name_(session_name),
    owner_id(owner_id) {
    member_list_.push_back(owner_id);

}

bool RESTfulChattingSessionInfo::join(const std::string& user_id) {
    for (auto user : member_list_) {
        if (user == user_id)
            return true;
    }

    member_list_.push_back(user_id);

    return true;
}

bool RESTfulChattingSessionInfo::leave(const std::string& user_id) {
    for (auto it = member_list_.begin(); it != member_list_.end(); it++) {
        if (*it == user_id) {
            member_list_.erase(it);
            return true;
        }
    }

    return false;
}

bool  RESTfulChattingSessionInfo::getMemberList(std::vector<std::string>& user_id_list) {
    for (auto user : member_list_)
        user_id_list.push_back(user);

    return true;
}

bool RESTfulChattingSessionInfo::newMessage(const std::string& user_id, const std::string& message) {
    std::shared_ptr<RESTfulChattingMessageInfo> new_message = std::make_shared<RESTfulChattingMessageInfo>();

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::stringstream ss;

#ifndef	LUNA_PLATFORM_RASPBERRY_PI2
    ss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
#else
    //	std::put_time can use in over gcc 5.2 version
#endif

    new_message->date_time_ = ss.str();
    new_message->from_id_ = user_id;
    new_message->message_ = message;

    return true;
}

//--
//  RESTfulChattingSessionInfo
//
bool RESTfulChattingSessionManager::create_id(const std::string& id, const std::string& name) {
    for (auto userInfo : user_info_list_) {
        if (userInfo->id_ == id) {
            std::cout << "[RESTfulChattingSessionManager::CreateID] '" << id << "' is a registered ID!" << std::endl;
            return false;
        }
    }

    std::shared_ptr<RESTfulChattingUserInfo> new_user = std::make_shared<RESTfulChattingUserInfo>();

    new_user->id_ = id;
    new_user->name_ = name;

    user_info_list_.push_back(new_user);

    std::cout << "[RESTfulChattingSessionManager::CreateID] '" << id << "'/'" << name << "'." << std::endl;

    return true;
}

bool RESTfulChattingSessionManager::delete_id(const std::string& id) {
    for (auto it = user_info_list_.begin(); it != user_info_list_.end(); it++) {
        if ((*it)->id_ == id) {
            std::cout << "[RESTfulChattingSessionManager::DeleteID] '" << id << "' was deleted!" << std::endl;

            user_info_list_.erase(it);
            return true;
        }
    }

    std::cout << "[RESTfulChattingSessionManager::DeleteID] '" << id << "' is not a registered ID!" << std::endl;

    return false;
}

bool RESTfulChattingSessionManager::create_session(const std::string& name, const std::string& owner_id) {
    for (auto session_info : session_info_list_) {
        if (session_info->name_ == name)
            return false;
    }

    std::shared_ptr<RESTfulChattingSessionInfo> new_session = std::make_shared<RESTfulChattingSessionInfo>(name, owner_id);

    session_info_list_.push_back(new_session);

    return true;
}

bool RESTfulChattingSessionManager::delete_session(const std::string& name, const std::string& owner_id) {
    for (auto it = session_info_list_.begin(); it != session_info_list_.end(); it++) {
        if ((*it)->name_ == name && (*it)->owner_id == owner_id) {
            session_info_list_.erase(it);
            return true;
        }
    }

    return false;
}

bool RESTfulChattingSessionManager::join_session(const std::string& name, const std::string& user_id) {
    for (auto session_info : session_info_list_) {
        if (session_info->name_ == name)
            return session_info->join(user_id);
    }

    return false;
}

bool RESTfulChattingSessionManager::leave_session(const std::string& name, const std::string& user_id) {
    for (auto session_info : session_info_list_) {
        if (session_info->name_ == name)
            return session_info->leave(user_id);
    }

    return false;
}

bool RESTfulChattingSessionManager::get_user_info(const std::string& id, RESTfulChattingUserInfo& user_info) {
    for (auto info : user_info_list_) {
        if (id == info->id_) {
            user_info = *info;

            return true;
        }
    }

    return false;
}


bool RESTfulChattingSessionManager::get_user_list(std::vector<std::string>& user_id_list) {
    user_id_list.clear();

    for (auto info : user_info_list_)
        user_id_list.push_back(info->id_);

    return true;
}

bool RESTfulChattingSessionManager::get_session_list(std::vector<std::string>& session_name_list, std::vector<std::string>& owner_id_list) {
    session_name_list.clear();

    for (auto info : session_info_list_) {
        session_name_list.push_back(info->name_);
        owner_id_list.push_back(info->owner_id);
    }

    return true;
}

bool RESTfulChattingSessionManager::get_session_info(const std::string& session_name, std::vector<std::string>& member_list) {
    for (auto session_info : session_info_list_) {
        if (session_info->name_ == session_name)
            return session_info->getMemberList(member_list);
    }

    return false;
}

bool  RESTfulChattingSessionManager::broadcast_message(const std::string& session_name, const std::string& user_id, const std::string& message) {

    return false;
}


/////
RESTfulChattingApiManager::RESTfulChattingApiManager() {
    // TODO Auto-generated constructor stub
    add_api(std::string("/user"), std::bind(&RESTfulChattingApiManager::user, this, std::placeholders::_1, std::placeholders::_2));
    add_api(std::string("/session"), std::bind(&RESTfulChattingApiManager::session, this, std::placeholders::_1, std::placeholders::_2));
    add_api(std::string("/session/member"), std::bind(&RESTfulChattingApiManager::session_member, this, std::placeholders::_1, std::placeholders::_2));
    add_api(std::string("/session/member/*"), std::bind(&RESTfulChattingApiManager::session_member_id, this, std::placeholders::_1, std::placeholders::_2));
    //  add_api(std::string("/session/message"), std::bind(&RESTfulChattingApiManager::session_message, this, std::placeholders::_1, std::placeholders::_2));
}

RESTfulChattingApiManager::~RESTfulChattingApiManager() {
    // TODO Auto-generated destructor stub
}

bool RESTfulChattingApiManager::user(std::shared_ptr<Luna::ccWebServerRequest> request, std::shared_ptr<Luna::ccWebServerResponse> response) {
    //std::cout << "enter RESTfulChattingApiManager::list, query_string=" << request->GetQueryString() << endl;

    switch (request->get_method()) {
    case Luna::ccWebServerRequest::HttpMethod_Post:   //  Create
    {
        Json::Reader    json_reader;
        Json::Value     root_value;

        std::string     json_data;

        json_data.reserve(1024);

        request->get_content_body(json_data);

        if (!json_reader.parse(json_data, root_value)) {
            response->send_status(500, std::string("Server Error!"));
            return false;
        }

        if (root_value["user_info"].isObject() == false) {
            response->send_status(500, std::string("Server Error!"));
            return false;
        }

        //  Json::Value oUserInfp = root_value["user_info"];
        if (chat_session_manager_.create_id(root_value["user_info"]["ID"].asString(), root_value["user_info"]["Name"].asString()) == true)
            response->send_status(201, std::string("Created"));
        else
            response->send_status(500, std::string("Server Error!"));

        response->send_content_type("application/x-www-form-urlencoded", (size_t)0);
    }
    break;

    case Luna::ccWebServerRequest::HttpMethod_Get:    //  Read
    {
        std::vector<std::string> member_list;

        chat_session_manager_.get_user_list(member_list);

        response->send_status(200, std::string("OK"));

        Json::Value oResponseJsonData;
        Json::Value oUserList;
        Json::StyledWriter json_writer;

        oUserList["count"] = (int)member_list.size();

        for (int nIndex = 0; nIndex < (int)member_list.size(); nIndex++)
            oUserList["info"][nIndex]["id"] = member_list[nIndex];

        oResponseJsonData["user_list"] = oUserList;

        std::string json_data = json_writer.write(oResponseJsonData);

        response->send_content_type("application/javascript", json_data.length());
        response->send_content(json_data);

        return true;
    }
    break;

    case Luna::ccWebServerRequest::HttpMethod_Delete:   //  Delete
    {
        std::string id = request->get_variable("ID");

        if (chat_session_manager_.delete_id(id) == true)
            response->send_status(202, std::string("OK"));
        else
            response->send_status(500, std::string("Server Error!"));

        response->send_content_type("application/x-www-form-urlencoded", (size_t)0);
    }
    break;

    default:
        response->send_status(500, std::string("Server Error!"));
        response->send_content_type("application/x-www-form-urlencoded", (size_t)0);
        break;

    }

    return false;
}

bool RESTfulChattingApiManager::session(std::shared_ptr<Luna::ccWebServerRequest> request, std::shared_ptr<Luna::ccWebServerResponse> response) {
    switch (request->get_method()) {
    case Luna::ccWebServerRequest::HttpMethod_Get:
    {
        std::cout << "enter RESTfulChattingApiManager::session" << std::endl;

        Json::Value json_value;
        Json::Value session_list_value;
        Json::StyledWriter json_writer;

        std::vector<std::string> session_name_list, owner_id_list;

        chat_session_manager_.get_session_list(session_name_list, owner_id_list);

        session_list_value["count"] = (int)session_name_list.size();

        for (int nIndex = 0; nIndex < (int)session_name_list.size(); nIndex++) {
            session_list_value["info"][nIndex]["Name"] = session_name_list[nIndex];
            session_list_value["info"][nIndex]["OwnerID"] = owner_id_list[nIndex];
        }

        json_value["session_list"] = session_list_value;

        std::string json_data = json_writer.write(json_value);

        response->send_status(200, std::string("OK"));
        response->send_content_type("application/javascript", json_data.length());
        response->send_content(json_data);
    }
    break;

    case Luna::ccWebServerRequest::HttpMethod_Post:
    {
        Json::Reader    json_reader;
        Json::Value     root_value;
        std::string     json_data;

        json_data.reserve(1024);

        request->get_content_body(json_data);

        if (!json_reader.parse(json_data, root_value)) {
            response->send_status(500, std::string("Server Error!"));
            return false;
        }

        if (root_value["session_info"].isObject() == false) {
            response->send_status(500, std::string("Server Error!"));
            return false;
        }

        std::string session_name = root_value["session_info"]["Name"].asString();
        std::string owner_id = root_value["session_info"]["OwnerID"].asString();

        if (chat_session_manager_.create_session(session_name, owner_id) == true) {
            response->send_status(201, std::string("Created"));

            std::string websocket_uri;

            Luna::ccString::format(websocket_uri, "/session/chat/%s", session_name.c_str());

            add_function(websocket_uri, std::bind(&RESTfulChattingApiManager::ws_chat, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
        } else
            response->send_status(500, std::string("Server Error!"));

        response->send_content_type("application/x-www-form-urlencoded", (size_t)0);
    }
    break;

    case Luna::ccWebServerRequest::HttpMethod_Delete:
    {
        Json::Reader    json_reader;
        Json::Value     root_value;

        std::string     json_data;

        json_data.reserve(1024);

        request->get_content_body(json_data);

        if (!json_reader.parse(json_data, root_value)) {
            response->send_status(500, std::string("Server Error!"));
            return false;
        }

        if (root_value["session_info"].isObject() == false) {
            response->send_status(500, std::string("Server Error!"));
            return false;
        }

        std::string session_name = root_value["session_info"]["Name"].asString();
        std::string owner_id = root_value["session_info"]["OwnerID"].asString();

        if (chat_session_manager_.delete_session(session_name, owner_id) == true) {
            response->send_status(202, std::string("OK"));

            std::string websocket_uri;

            Luna::ccString::format(websocket_uri, "/session/chat/%s", session_name.c_str());

            remove_function(websocket_uri);
        } else
            response->send_status(500, std::string("Server Error!"));

        response->send_content_type("application/x-www-form-urlencoded", (size_t)0);
    }
    break;

    default:
        response->send_status(500, std::string("Server Error!"));
        response->send_content_type("application/x-www-form-urlencoded", (size_t)0);
        break;

    }

    return true;
}

bool RESTfulChattingApiManager::session_member(std::shared_ptr<Luna::ccWebServerRequest> request, std::shared_ptr<Luna::ccWebServerResponse> response) {
    switch (request->get_method()) {
    case Luna::ccWebServerRequest::HttpMethod_Get:
    {
        std::string session_name = request->get_variable("Name");
        std::string user_id = request->get_variable("UserID");

        std::vector<std::string> aMemberList;

        chat_session_manager_.get_session_info(session_name, aMemberList);

        Json::Value json_value;
        Json::Value session_member_list;
        Json::StyledWriter json_writer;
        session_member_list["Name"] = session_name;
        session_member_list["count"] = (int)aMemberList.size();

        for (int nIndex = 0; nIndex < (int)aMemberList.size(); nIndex++)
            session_member_list["info"][nIndex]["ID"] = aMemberList[nIndex];

        json_value["session_member_list"] = session_member_list;

        std::string json_data = json_writer.write(json_value);

        response->send_status(200, std::string("OK"));
        response->send_content_type("application/javascript", json_data.length());
        response->send_content(json_data);
    }
    break;

    case Luna::ccWebServerRequest::HttpMethod_Put:    //  join
    {
        std::string session_name = request->get_variable("Name");
        std::string user_id = request->get_variable("UserID");

        if (chat_session_manager_.join_session(session_name, user_id) == true)
            response->send_status(200, std::string("OK"));
        else
            response->send_status(500, std::string("Server Error!"));

        response->send_content_type("application/x-www-form-urlencoded", (size_t)0);
    }
    break;

    case Luna::ccWebServerRequest::HttpMethod_Delete:
    {
        std::string session_name = request->get_variable("Name");
        std::string user_id = request->get_variable("UserID");

        if (chat_session_manager_.leave_session(session_name, user_id) == true)
            response->send_status(202, std::string("OK"));
        else
            response->send_status(500, std::string("Server Error!"));

        response->send_content_type("application/x-www-form-urlencoded", (size_t)0);
    }
    break;

    default:
        response->send_status(500, std::string("Server Error!"));
        response->send_content_type("application/x-www-form-urlencoded", (size_t)0);
        break;
    }

    return true;
}

bool RESTfulChattingApiManager::session_member_id(std::shared_ptr<Luna::ccWebServerRequest> request, std::shared_ptr<Luna::ccWebServerResponse> response) {
    //  I'll implement this method as soon as possible.
    std::string user_id = request->get_resource();

    response->send_status(500, std::string("Server Error!"));
    response->send_content_type("application/x-www-form-urlencoded", (size_t)0);

    return true;
}

//  
bool RESTfulChattingApiManager::ws_chat(Luna::ccWebsocket::ccWebSocketEvent ws_event, std::shared_ptr<Luna::ccWebsocket> websocket, const char* data, uint32_t size, bool is_text) {
    switch ((int)ws_event) {
    case Luna::ccWebsocket::ccWebSocketEvent_Connected:
    {
        std::string message;

        Luna::ccString::format(message, "%08x: Join", websocket->get_instance());

        websocket->get_group()->broadcast(message);
        break;
    }

    case Luna::ccWebsocket::ccWebSocketEvent_ReceivedData:
    {
        std::string broadcast_message;

        std::string recv_text(data, size);

        Luna::ccString::format(broadcast_message, "%08x: %s", websocket->get_instance(), recv_text.c_str());

        websocket->get_group()->broadcast(broadcast_message);
        break;
    }

    case Luna::ccWebsocket::ccWebSocketEvent_Disconnected:
    {
        std::string message;

        Luna::ccString::format(message, "%08x: Leave", websocket->get_instance());

        websocket->get_group()->broadcast(message);
        break;
    }
    }

    return true;
}
