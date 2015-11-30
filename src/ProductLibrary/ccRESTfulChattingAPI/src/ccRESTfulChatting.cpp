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
            return false;
    }

    _aMembers.push_back(strUserID);

    return true;
}

bool ccRESTfulChattingSessionInfo::Leave(const std::string& strUserID)
{
    std::vector<std::string>::iterator  it;

    for (it = _aMembers.begin(); it != _aMembers.end(); it++)
    {
        if (*it == strUserID)
        {
            _aMembers.erase(it);
            return true;
        }
    }

    return false;
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
    std::vector<std::shared_ptr<ccRESTfulChattingUserInfo>>::iterator  it;

    for (it = _aUserInfos.begin(); it != _aUserInfos.end(); it++)
    {
        if ((*it)->_strID == strID)
        {
            _aUserInfos.erase(it);
            return true;
        }
    }

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
    std::vector<std::shared_ptr<ccRESTfulChattingSessionInfo>>::iterator  it;

    for (it = _aSessionInfos.begin(); it != _aSessionInfos.end(); it++)
    {
        if ((*it)->_strName == strName && (*it)->_strOwnerID == strOwnerID)
        {
            _aSessionInfos.erase(it);
            return true;
        }
    }

    return false;
}

bool ccRESTfulChattingSessionManager::JoinSession(const std::string& strName, const std::string& strOwnerID)
{


    return false;

}

bool ccRESTfulChattingSessionManager::LeaveSession(const std::string& strName, const std::string& strOwnerID)
{

    return false;
}

bool ccRESTfulChattingSessionManager::GetUserList(std::vector<std::string>& aUserIDs)
{
    aUserIDs.clear();

    for (auto info : _aUserInfos)
        aUserIDs.push_back(info->_strID);

    return true;
}

bool ccRESTfulChattingSessionManager::GetSessionList(std::vector<std::string>& aSessionNames)
{
    aSessionNames.clear();

    for (auto info : _aSessionInfos)
        aSessionNames.push_back(info->_strName);

    return true;
}

bool ccRESTfulChattingSessionManager::GetSessionInfo(const std::string& strSessionName, std::vector<std::string>& aMembers)
{

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
    AddAPI(std::string("/session/message"), std::bind(&ccRESTfulChatting::session_message, this, std::placeholders::_1, std::placeholders::_2));
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

            std::string     strJsonRPC;

            strJsonRPC.reserve(1024);

            pRequest->GetContentBody(strJsonRPC);

            if (!oReader.parse(strJsonRPC, oRootValue))
            {
                pResponse->Status(500, string("Server Error!"));
                return false;
            }

            if (oRootValue["jsonrpc"] != "2.0")
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
                pResponse->Status(200, string("OK)"));
            else
                pResponse->Status(500, string("Server Error!"));

            //static int nAutoCount = 0;

            //ccString    strNewID;
            //ccString    strNewName;

            //ccString::format(strNewID, "ID=%03d", nAutoCount);
            //ccString::format(strNewName, "Name=%03d", nAutoCount);

            //nAutoCount++;

            //if (_oSessionManager.CreateID(strNewID, strNewName) == true)
            //    pResponse->Status(200, string("OK)"));
            //else
            //    pResponse->Status(500, string("Server Error!"));
        }
        break;

    case ccWebServerRequest::HttpMethod_Get:    //  Read
        {
            std::vector<std::string> aMembers;

            _oSessionManager.GetUserList(aMembers);

            pResponse->Status(200, string("OK)"));

            Json::Value oResponseRPC;
            Json::Value oUserList;
            Json::StyledWriter oWriter;
            
            oResponseRPC["jsonrpc"] = "2.0";
            oResponseRPC["id"]      = 1;

            oUserList["count"] = aMembers.size();

            for (int nIndex = 0; nIndex < aMembers.size(); nIndex++)
            {
                oUserList["info"][nIndex]["id"] = aMembers[nIndex];
            }

            oResponseRPC["user_list"] = oUserList;

            std::string strJsonRPC2 = oWriter.write(oResponseRPC);

            pResponse->ContentType("application/javascript", strJsonRPC2.length());
            pResponse->Write(strJsonRPC2);

            return true;
        }
        break;

    case ccWebServerRequest::HttpMethod_Delete:   //  Delete
        {
            static int nAutoCount = 0;

            ccString    strNewID;
            ccString    strNewName;

            ccString::format(strNewID, "ID=%03d", nAutoCount);

            nAutoCount++;

            if (_oSessionManager.DeleteID(strNewID) == true)
                pResponse->Status(200, string("OK)"));
            else
                pResponse->Status(500, string("Server Error!"));
        }
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

        pResponse->Status(200, string("OK)"));

        Json::Value oResponseRPC;
        Json::Value oSessionList;
        Json::StyledWriter oWriter;

        oResponseRPC["jsonrpc"] = "2.0";
        oResponseRPC["id"] = 1;

        oSessionList["count"] = 3;
        oSessionList["info"][0]["name"] = "session1";
        oSessionList["info"][0]["member_count"] = 4;
        oSessionList["info"][1]["name"] = "session #2";
        oSessionList["info"][1]["member_count"] = 2;
        oSessionList["info"][2]["name"] = "session ####4";
        oSessionList["info"][2]["member_count"] = 10;

        oResponseRPC["session_list"] = oSessionList;

        std::string strJsonRPC2 = oWriter.write(oResponseRPC);

        pResponse->ContentType("application/javascript", strJsonRPC2.length());
        pResponse->Write(strJsonRPC2);

        break;
    }
           
    }

    return false;
}

bool ccRESTfulChatting::session_member(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)
{

    return false;
}

bool ccRESTfulChatting::session_message(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)
{

    return false;
}
