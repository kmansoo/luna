#pragma once

#include "HSCoreAPI/HSObject.h"
#include "HSCoreAPI/HSMutex.h"
#include "HSCoreAPI/HSSingletonT.h"
#include "HSCoreAPI/HSTimerThread.h"

#include "HSJsonAPI/json/json.h"

#include "IHSWin32RgWebApiTransactionNotifier.h"


class ccWin32WebApiHelper : 
    public HSTimerThread
{
protected:
    ccWin32WebApiHelper(void);

public:
    virtual ~ccWin32WebApiHelper(void);

public:
    HS_SINGLETON_IMPL(ccWin32WebApiHelper);

public:
    void    SetConnectionInfo(const CString strIP, UINT uPort);
    void    SetToken(const CString strToken);

    BOOL    RequestAPI(const CString& strFunction, CString& strResponse);
    BOOL    RequestAPI(const CString& strFunction, Json::Value& oParams, CString& strResponse);

    BOOL    AsyncRequestAPI(const CString& strFunction, IHSWin32RgWebApiTransactionNotifier *pNotifier = NULL);
    BOOL    AsyncRequestAPI(const CString& strFunction, Json::Value& oParams, IHSWin32RgWebApiTransactionNotifier *pNotifier = NULL);

public:
    static BOOL getValueString(Json::Value& oValue, const char* strName, CString& strValue, CString strDefaultValue = _T(""));
    static BOOL getValueInt(Json::Value& oValue, const char* strName, int& nValue, int nDefaultValue = 0);

private:
    BOOL    DoMakeRequestAPI(CString& strAPI, const CString& strFunction, Json::Value& oParams);
    BOOL    DoSendRequestAPI(const CString& strAPI, CString& strResponse);

protected:
    enum TIMER_ID {
        TID_CHECK_TRANSACTION = 0x100,
    };

    virtual void    OnHSTimer(EtDWORD uIDEvent);

private:
    class   XTransactionInfo : public HSObject
    {
    public:
        XTransactionInfo() {_pNotifier = NULL;}
        virtual ~XTransactionInfo() {}

    public:
        CString         _strAPI;

        IHSWin32RgWebApiTransactionNotifier* _pNotifier;
    };

    typedef sp<XTransactionInfo>    XTransactionInfoT;

private:
    CString     _strDestIP;
    UINT        _uDestPort;
    CString     _strToken;

    HSRefObjVectorT<XTransactionInfoT, XTransactionInfoT&>    _aTransactionInfos;

    HSMutex     _mtx;
};
