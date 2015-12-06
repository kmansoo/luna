/*
 * ccRESTfulChattingApi.h
 *
 *  Created on: 2015. 11. 28.
 *      Author: kmansoo
 */

#ifndef CCPRODUCTLIBRARY_CCRESTFULCHATTINGAPI_CCRESTFULCHATTING_H_
#define CCPRODUCTLIBRARY_CCRESTFULCHATTINGAPI_CCRESTFULCHATTING_H_

#include "ccWebServerAPI/ccRESTfulApi.h"

using namespace std;

class ccRESTfulChattingUserInfo
{
public:
    std::string     _strID;
    std::string     _strName;
};

class ccRESTfulChattingMessageInfo
{
public:
    std::string     _strDateTime;
    std::string     _strFromID;
    std::string     _strMessage;
};

class ccRESTfulChattingSessionInfo
{
public:
    ccRESTfulChattingSessionInfo(const std::string& strSessionName, const std::string& strOwnerID);
    ~ccRESTfulChattingSessionInfo() {}

public:
    bool    Join(const std::string& strUserID);
    bool    Leave(const std::string& strUserID);
    bool    GetMemberList(std::vector<std::string>& aUserIDs);

    bool    NewMessage(const std::string& strUserID, const std::string& strMessage);

public:
    std::string     _strName;
    std::string     _strOwnerID;

private:
    std::vector<std::string>    _aMembers;
    std::vector<std::shared_ptr<ccRESTfulChattingMessageInfo>>  _aMessages;
};

class ccRESTfulChattingSessionManager
{
public:
    ccRESTfulChattingSessionManager() {}

public:
    bool    CreateID(const std::string& strID, const std::string& strName);
    bool    DeleteID(const std::string& strID);
    bool    GetUserList(std::vector<std::string>& aUserIDs);

    bool    CreateSession(const std::string& strName, const std::string& strOwnerID);
    bool    DeleteSession(const std::string& strName, const std::string& strRequestID);

    bool    JoinSession(const std::string& strSessionName, const std::string& strUserID);
    bool    LeaveSession(const std::string& strSessionName, const std::string& strUserID);

    bool    GetSessionList(std::vector<std::string>& aSessionNames, std::vector<std::string>& aOwnerIDs);
    bool    GetSessionInfo(const std::string& strSessionName, std::vector<std::string>& aMembers);

    bool    BroadcastMessage(const std::string& strSessionName, const std::string& strUserID, const std::string& strMessage);

protected:
    std::shared_ptr<ccRESTfulChattingSessionInfo> DoFindSession(const std::string& strName);

private:
    std::vector< std::shared_ptr<ccRESTfulChattingUserInfo>>    _aUserInfos;
    std::vector< std::shared_ptr<ccRESTfulChattingSessionInfo>> _aSessionInfos;
};

class ccRESTfulChatting : public ccRESTfulApi
{
public:
    ccRESTfulChatting();
    virtual ~ccRESTfulChatting();

protected:
    bool    user(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse);
    bool    session(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse);
    bool    session_member(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse);
    bool    session_message(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse);

private:
    ccRESTfulChattingSessionManager   _oSessionManager;
};


#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVER_H_ */
