#pragma once

#include <memory>
#include <functional>
#include <mutex>
#include <thread>

#include "ccWebServerAPI/ccWebServerRequest.h"
#include "ccJsonParserAPI/json/json.h"
#include "ccWin32RgWebApiTransactionNotifier.h"


class ccWin32WebApiHelper
{
protected:
    ccWin32WebApiHelper(void);

public:
    virtual ~ccWin32WebApiHelper(void);

public:
    static std::shared_ptr<ccWin32WebApiHelper> getInstance();

public:
    void    SetConnectionInfo(const std::string& strIP, UINT uPort);

    bool    RequestAPI(ccWebServerRequest::HttpMethod eMethod, const std::string& strWebAPI, std::string& strResponse);
    bool    RequestAPI(ccWebServerRequest::HttpMethod eMethod, const std::string& strWebAPI, Json::Value& oParams, std::string& strResponse);

    bool    AsyncRequestAPI(ccWebServerRequest::HttpMethod eMethod, const std::string& strWebAPI, ccWin32RgWebApiTransactionNotifier *pNotifier = NULL);
    bool    AsyncRequestAPI(ccWebServerRequest::HttpMethod eMethod, const std::string& strWebAPI, Json::Value& oParams, ccWin32RgWebApiTransactionNotifier *pNotifier = NULL);

public:
    static bool getValueString(Json::Value& oValue, const std::string& strName, std::string& strValue, std::string strDefaultValue = _T(""));
    static bool getValueInt(Json::Value& oValue, const std::string& strName, int& nValue, int nDefaultValue = 0);

private:
    bool    DoSendRequestAPI(ccWebServerRequest::HttpMethod eMethod, const std::string& strWebAPI, const std::string& strRequestData, std::string& strResponse);

private:
    void    DoRunThread();

private:
    std::thread*    _pPollThread;
    bool            _bIsStopThread;

private:
    class   XTransactionInfo
    {
    public:
        XTransactionInfo() {_pNotifier = NULL;}
        virtual ~XTransactionInfo() {}

    public:
        ccWebServerRequest::HttpMethod  _eMethod;
        std::string                     _strWebAPI;
        std::string                     _strRequestData;

        ccWin32RgWebApiTransactionNotifier* _pNotifier;
    };

private:
    std::string     _strDestIP;
    UINT            _uDestPort;

    std::vector<std::shared_ptr<XTransactionInfo>>   _aTransactionInfos;

    std::mutex      _mutex;
};
