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

#include "ccCoreAPI/ccString.h"

#include "ccJsonParserAPI/json/value.h"
#include "ccJsonParserAPI/json/writer.h"
#include "ccJsonParserAPI/json/reader.h"

#include "../ccRESTfulChatting.h"

///
ccRESTfulChattingSessionInfo::ccRESTfulChattingSessionInfo(const std::string& strSessionName, const std::string& strOwnerID) : 
    _strName(strSessionName),
    _strOwnerID(strOwnerID)
{
    _aMembers.push_back(strOwnerID);
 
}

bool ccRESTfulChattingSessionInfo::Join(const std::string& strUserID)
{
    for (auto user : _aMembers)
    {
        if (user == strUserID)
            return true;
    }

    _aMembers.push_back(strUserID);

    return true;
}

bool ccRESTfulChattingSessionInfo::Leave(const std::string& strUserID)
{
    for (auto it = _aMembers.begin(); it != _aMembers.end(); it++)
    {
        if (*it == strUserID)
        {
            _aMembers.erase(it);
            return true;
        }
    }

    return false;
}

bool  ccRESTfulChattingSessionInfo::GetMemberList(std::vector<std::string>& aUserIDs)
{
    for (auto user : _aMembers)
        aUserIDs.push_back(user);

    return true;
}

bool ccRESTfulChattingSessionInfo::NewMessage(const std::string& strUserID, const std::string& strMessage)
{
    std::shared_ptr<ccRESTfulChattingMessageInfo> pNewMessage(new ccRESTfulChattingMessageInfo);

    auto t  = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::stringstream ss;
    
    ss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");

    pNewMessage->_strDateTime   = ss.str();
    pNewMessage->_strFromID     = strUserID;
    pNewMessage->_strMessage    = strMessage;

    return true;
}

//--
//  ccRESTfulChattingSessionInfo
//
bool ccRESTfulChattingSessionManager::CreateID(const std::string& strID, const std::string& strName)
{
    for (auto userInfo : _aUserInfos)
    {
        if (userInfo->_strID == strID)
        {
            std::cout << "[ccRESTfulChattingSessionManager::CreateID] '" << strID << "' is a registered ID!" << std::endl;
            return false;
        }
    }

    std::shared_ptr<ccRESTfulChattingUserInfo> pNewUser(new ccRESTfulChattingUserInfo);

    pNewUser->_strID = strID;
    pNewUser->_strName = strName;

    _aUserInfos.push_back(pNewUser);

    std::cout << "[ccRESTfulChattingSessionManager::CreateID] '" << strID << "'/'" << strName << "'." << std::endl;

    return true;
}

bool ccRESTfulChattingSessionManager::DeleteID(const std::string& strID)
{
    for (auto it = _aUserInfos.begin(); it != _aUserInfos.end(); it++)
    {
        if ((*it)->_strID == strID)
        {
            std::cout << "[ccRESTfulChattingSessionManager::DeleteID] '" << strID << "' was deleted!" << std::endl;

            _aUserInfos.erase(it);
            return true;
        }
    }

    std::cout << "[ccRESTfulChattingSessionManager::DeleteID] '" << strID << "' is not a registered ID!" << std::endl;

    return false;
}

bool ccRESTfulChattingSessionManager::CreateSession(const std::string& strName, const std::string& strOwnerID)
{
    for (auto sessionInfo : _aSessionInfos)
    {
        if (sessionInfo->_strName == strName)
            return false;
    }

    std::shared_ptr<ccRESTfulChattingSessionInfo> pNewSession(new ccRESTfulChattingSessionInfo(strName, strOwnerID));

    _aSessionInfos.push_back(pNewSession);

    return true;
}

bool ccRESTfulChattingSessionManager::DeleteSession(const std::string& strName, const std::string& strOwnerID)
{
    for (auto it = _aSessionInfos.begin(); it != _aSessionInfos.end(); it++)
    {
        if ((*it)->_strName == strName && (*it)->_strOwnerID == strOwnerID)
        {
            _aSessionInfos.erase(it);
            return true;
        }
    }

    return false;
}

bool ccRESTfulChattingSessionManager::JoinSession(const std::string& strName, const std::string& strUserID)
{
    for (auto sessionInfo : _aSessionInfos)
    {
        if (sessionInfo->_strName == strName)
            return sessionInfo->Join(strUserID);
    }

    return false;
}

bool ccRESTfulChattingSessionManager::LeaveSession(const std::string& strName, const std::string& strUserID)
{
    for (auto sessionInfo : _aSessionInfos)
    {
        if (sessionInfo->_strName == strName)
            return sessionInfo->Leave(strUserID);
    }

    return false;
}

bool ccRESTfulChattingSessionManager::GetUserList(std::vector<std::string>& aUserIDs)
{
    aUserIDs.clear();

    for (auto info : _aUserInfos)
        aUserIDs.push_back(info->_strID);

    return true;
}

bool ccRESTfulChattingSessionManager::GetSessionList(std::vector<std::string>& aSessionNames, std::vector<std::string>& aOwnerIDs)
{
    aSessionNames.clear();

    for (auto info : _aSessionInfos)
    {
        aSessionNames.push_back(info->_strName);
        aOwnerIDs.push_back(info->_strOwnerID);
    }

    return true;
}

bool ccRESTfulChattingSessionManager::GetSessionInfo(const std::string& strSessionName, std::vector<std::string>& aMembers)
{
    for (auto sessionInfo : _aSessionInfos)
    {
        if (sessionInfo->_strName == strSessionName)
            return sessionInfo->GetMemberList(aMembers);
    }


    return false;
}

bool  ccRESTfulChattingSessionManager::BroadcastMessage(const std::string& strSessionName, const std::string& strUserID, const std::string& strMessage)
{

    return false;
}


/////
ccRESTfulChatting::ccRESTfulChatting()
{
    // TODO Auto-generated constructor stub
    AddAPI(std::string("/user"), std::bind(&ccRESTfulChatting::user, this, std::placeholders::_1, std::placeholders::_2));
    AddAPI(std::string("/session"), std::bind(&ccRESTfulChatting::session, this, std::placeholders::_1, std::placeholders::_2));
    AddAPI(std::string("/session/member"), std::bind(&ccRESTfulChatting::session_member, this, std::placeholders::_1, std::placeholders::_2));
    //  AddAPI(std::string("/session/message"), std::bind(&ccRESTfulChatting::session_message, this, std::placeholders::_1, std::placeholders::_2));
}

ccRESTfulChatting::~ccRESTfulChatting()
{
    // TODO Auto-generated destructor stub
}

bool ccRESTfulChatting::user(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)
{
    //std::cout << "enter ccRESTfulChatting::list, query_string=" << pRequest->GetQueryString() << endl;

    switch (pRequest->GetMethod())
    {
    case ccWebServerRequest::HttpMethod_Post:   //  Create
        {
            Json::Reader    oReader;
            Json::Value     oRootValue;

            std::string     strJsonData;

            strJsonData.reserve(1024);

            pRequest->GetContentBody(strJsonData);

            if (!oReader.parse(strJsonData, oRootValue))
            {
                pResponse->Status(500, string("Server Error!"));
                return false;
            }

            if (oRootValue["user_info"].isObject() == false)
            {
                pResponse->Status(500, string("Server Error!"));
                return false;
            }

            //  Json::Value oUserInfp = oRootValue["user_info"];
            if (_oSessionManager.CreateID(oRootValue["user_info"]["ID"].asString(), oRootValue["user_info"]["Name"].asString()) == true)
                pResponse->Status(201, string("Created"));
            else
                pResponse->Status(500, string("Server Error!"));
    
            pResponse->ContentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        }
        break;

    case ccWebServerRequest::HttpMethod_Get:    //  Read
        {
            std::vector<std::string> aMembers;

            _oSessionManager.GetUserList(aMembers);

            pResponse->Status(200, string("OK"));

            Json::Value oResponseJsonData;
            Json::Value oUserList;
            Json::StyledWriter oWriter;
            
            oUserList["count"] = aMembers.size();

            for (int nIndex = 0; nIndex < aMembers.size(); nIndex++)
                oUserList["info"][nIndex]["id"] = aMembers[nIndex];

            oResponseJsonData["user_list"] = oUserList;

            std::string strJsonData = oWriter.write(oResponseJsonData);

            pResponse->ContentType("application/javascript", strJsonData.length());
            pResponse->Write(strJsonData);

            return true;
        }
        break;

    case ccWebServerRequest::HttpMethod_Delete:   //  Delete
        {
            std::string strID = pRequest->GetVar("ID");

            if (_oSessionManager.DeleteID(strID) == true)
                pResponse->Status(202, string("OK"));
            else
                pResponse->Status(500, string("Server Error!"));
    
            pResponse->ContentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
    }
        break;

    default:
        pResponse->Status(500, string("Server Error!"));
        pResponse->ContentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        break;

    }

    return false;
}

bool ccRESTfulChatting::session(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)
{
    switch (pRequest->GetMethod())
    {
    case ccWebServerRequest::HttpMethod_Get:
        {
            std::cout << "enter ccRESTfulChatting::session" << std::endl;

            Json::Value oResponseJsonValue;
            Json::Value oSessionList;
            Json::StyledWriter oWriter;

            std::vector<std::string> aSessionNames, aOwnerIDs;

            _oSessionManager.GetSessionList(aSessionNames, aOwnerIDs);

            oSessionList["count"] = aSessionNames.size();

            for (int nIndex = 0; nIndex < aSessionNames.size(); nIndex++)
            {
                oSessionList["info"][nIndex]["Name"] = aSessionNames[nIndex];
                oSessionList["info"][nIndex]["OwnerID"] = aOwnerIDs[nIndex];
            }

            oResponseJsonValue["session_list"] = oSessionList;

            std::string strJsonData = oWriter.write(oResponseJsonValue);

            pResponse->Status(200, string("OK"));
            pResponse->ContentType("application/javascript", strJsonData.length());
            pResponse->Write(strJsonData);
        }
        break;

    case ccWebServerRequest::HttpMethod_Post:
        {
            Json::Reader    oReader;
            Json::Value     oRootValue;

            std::string     strJsonData;

            strJsonData.reserve(1024);

            pRequest->GetContentBody(strJsonData);

            if (!oReader.parse(strJsonData, oRootValue))
            {
                pResponse->Status(500, string("Server Error!"));
                return false;
            }

            if (oRootValue["session_info"].isObject() == false)
            {
                pResponse->Status(500, string("Server Error!"));
                return false;
            }

            std::string strSessionName = oRootValue["session_info"]["Name"].asString();
            std::string strOwnerID = oRootValue["session_info"]["OwnerID"].asString();

            if (_oSessionManager.CreateSession(strSessionName, strOwnerID) == true)
            {
                pResponse->Status(201, string("Created"));

                std::string strWSUrl;

                ccString::format(strWSUrl, "/session/chat/%s", strSessionName.c_str());
                
                AddFunction(strWSUrl, std::bind(&ccRESTfulChatting::ws_chat, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
            }
            else
                pResponse->Status(500, string("Server Error!"));
    
            pResponse->ContentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        }
        break;

    case ccWebServerRequest::HttpMethod_Delete:
        {
            Json::Reader    oReader;
            Json::Value     oRootValue;

            std::string     strJsonData;

            strJsonData.reserve(1024);

            pRequest->GetContentBody(strJsonData);

            if (!oReader.parse(strJsonData, oRootValue))
            {
                pResponse->Status(500, string("Server Error!"));
                return false;
            }

            if (oRootValue["session_info"].isObject() == false)
            {
                pResponse->Status(500, string("Server Error!"));
                return false;
            }

            std::string strSessionName = oRootValue["session_info"]["Name"].asString();
            std::string strOwnerID = oRootValue["session_info"]["OwnerID"].asString();

            if (_oSessionManager.DeleteSession(strSessionName, strOwnerID) == true)
            {
                pResponse->Status(202, string("OK"));

                std::string strWSUrl;

                ccString::format(strWSUrl, "/session/chat/%s", strSessionName.c_str());
                
                RemoveFunction(strWSUrl);
            }
            else
                pResponse->Status(500, string("Server Error!"));

            pResponse->ContentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        }
        break;

    default:
        pResponse->Status(500, string("Server Error!"));
        pResponse->ContentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        break;

    }

    return true;
}

bool ccRESTfulChatting::session_member(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)
{   
    switch (pRequest->GetMethod())
    {
        case ccWebServerRequest::HttpMethod_Get:
        {
            std::string strSessionName = pRequest->GetVar("Name");
            std::string strUserID = pRequest->GetVar("UserID");

            std::vector<std::string> aMemberList;

            _oSessionManager.GetSessionInfo(strSessionName, aMemberList);

            Json::Value oResponseJsonValue;
            Json::Value oSessionMemberList;
            Json::StyledWriter oWriter;
            oSessionMemberList["Name"] = strSessionName;
            oSessionMemberList["count"] = aMemberList.size();

            for (int nIndex = 0; nIndex < aMemberList.size(); nIndex++)
                oSessionMemberList["info"][nIndex]["ID"] = aMemberList[nIndex];

            oResponseJsonValue["session_member_list"] = oSessionMemberList;

            std::string strJsonData = oWriter.write(oResponseJsonValue);

            pResponse->Status(200, string("OK"));
            pResponse->ContentType("application/javascript", strJsonData.length());
            pResponse->Write(strJsonData);
        }
        break;

        case ccWebServerRequest::HttpMethod_Put:    //  join
        {
            std::string strSessionName = pRequest->GetVar("Name");
            std::string strUserID = pRequest->GetVar("UserID");

            if (_oSessionManager.JoinSession(strSessionName, strUserID) == true)
                pResponse->Status(200, string("OK"));
            else
                pResponse->Status(500, string("Server Error!"));

            pResponse->ContentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        }
        break;

        case ccWebServerRequest::HttpMethod_Delete:
        {
            std::string strSessionName = pRequest->GetVar("Name");
            std::string strUserID = pRequest->GetVar("UserID");

            if (_oSessionManager.LeaveSession(strSessionName, strUserID) == true)
                pResponse->Status(202, string("OK"));
            else
                pResponse->Status(500, string("Server Error!"));

            pResponse->ContentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        }
        break;

        default:
            pResponse->Status(500, string("Server Error!"));
            pResponse->ContentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
            break;
    }

    return true;
}

//  
bool ccRESTfulChatting::ws_chat(ccWebsocket::ccWebSocketEvent eEvent, std::shared_ptr<ccWebsocket> pWS, const char* strData, std::size_t size)
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

//bool ccRESTfulChatting::session_message(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)
//{
//
//    pResponse->Status(500, string("Server Error!"));
//    pResponse->ContentType("Content-Type: application/x-www-form-urlencoded", (size_t)0);
//
//    return false;
//}
