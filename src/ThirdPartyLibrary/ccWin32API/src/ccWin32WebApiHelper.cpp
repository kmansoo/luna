#include "stdafx.h"

#include "afxinet.h"

#include "ccWin32WebApiHelper.h"

ccWin32WebApiHelper::ccWin32WebApiHelper(void) : 
    HSTimerThread("BBRgWebApiManager"),
    _strDestIP(_T("192.168.0.1")), 
    _uDestPort(80)
{
    SetTimer(TID_CHECK_TRANSACTION, 1);
}

ccWin32WebApiHelper::~ccWin32WebApiHelper(void)
{
}

void ccWin32WebApiHelper::SetConnectionInfo(const CString strIP, UINT uPort)
{
    _strDestIP  =   strIP;
    _uDestPort  =   uPort;
}

void ccWin32WebApiHelper::SetToken(const CString strToken)
{
    _strToken  =   strToken;
}

BOOL ccWin32WebApiHelper::RequestAPI(const CString& strFunction, CString& strResponse)
{
    CString     strAPI;
    Json::Value oParams;

    if (DoMakeRequestAPI(strAPI, strFunction, oParams) == FALSE)
        return FALSE;

    return DoSendRequestAPI(strAPI, strResponse);
}

BOOL ccWin32WebApiHelper::RequestAPI(const CString& strFunction, Json::Value& oParams, CString& strResponse)
{
    CString     strAPI;

    if (DoMakeRequestAPI(strAPI, strFunction, oParams) == FALSE)
        return FALSE;

    return DoSendRequestAPI(strAPI, strResponse);
}

BOOL ccWin32WebApiHelper::AsyncRequestAPI(const CString& strFunction, IHSWin32RgWebApiTransactionNotifier *pNotifier)
{
    Json::Value oParams;

    return AsyncRequestAPI(strFunction, oParams, pNotifier);
}

BOOL ccWin32WebApiHelper::AsyncRequestAPI(const CString& strFunction, Json::Value& oParams, IHSWin32RgWebApiTransactionNotifier *pNotifier)
{
    if (pNotifier == NULL)
        return FALSE;

    sp<XTransactionInfo>    pNewTransaction(new XTransactionInfo, true);

    pNewTransaction->_pNotifier = pNotifier;

    if (DoMakeRequestAPI(pNewTransaction->_strAPI, strFunction, oParams) == FALSE)
        return FALSE;

    _mtx.Lock();

    BOOL bExist = FALSE;

    for (int nIndex = 0; nIndex < _aTransactionInfos.Size(); nIndex++)
    {
        if (_aTransactionInfos[nIndex]->_pNotifier == pNotifier)
        {
            bExist = TRUE;
            break;
        }
    }

    if (!bExist)
        _aTransactionInfos.Add(pNewTransaction);

    _mtx.Unlock();

    return TRUE;
}

BOOL ccWin32WebApiHelper::DoSendRequestAPI(const CString& strAPI, CString& strResponse)
{
    USES_CONVERSION;  

    CInternetSession    oSession(_T("MySession"));  

    CHttpFile*          pHttpFile   = NULL;  
    CHttpConnection*    pHttpConn   = NULL;  

    BOOL                bRetValue   = FALSE;
    CString             sURL        = _T("");  
    DWORD               dwSvcType   = 0;  
    DWORD               dwFlags     = INTERNET_FLAG_EXISTING_CONNECT;

    sURL.Format(__T("http://%s:%d/api"), _strDestIP, _uDestPort);

    CString             strSendData;
    CString             sServer, sObject;
    CString             sReqHdr;  

    INTERNET_PORT       nPort = 0;  

    if (AfxParseURL(sURL, dwSvcType, sServer, sObject, nPort) == FALSE)
        return bRetValue;

    try {
        oSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 5000);
        pHttpConn = oSession.GetHttpConnection(sServer, nPort);

        if (pHttpConn == NULL)
            goto clearVariabls;

        pHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_POST, sObject, NULL, 1, NULL, NULL, dwFlags);

        if (pHttpFile == NULL)
            goto clearVariabls;

        pHttpFile->SetReadBufferSize(4096);
    }
    catch (CInternetException* e)
    {
        TCHAR szError[255];

        e->GetErrorMessage(szError,255);
        e->Delete();

        goto clearVariabls;
    }

    sReqHdr.Format(_T("Content-Type: application/x-www-form-urlencoded"));  
    pHttpFile->AddRequestHeaders(sReqHdr);  

    //  Request
    strResponse = _T("");

    BOOL bRet = FALSE;
    char szResponse[5000];  

    try {

        bRet = pHttpFile->SendRequestEx(strAPI.GetLength(), HSR_ASYNC | HSR_INITIATE);  

        pHttpFile->Write(CT2A(strAPI), strAPI.GetLength());   

        bRet = pHttpFile->EndRequest(HSR_ASYNC);  

        ULONGLONG dwResponseLength = pHttpFile->GetLength();  

        while (0 != dwResponseLength)
        {
            int nRead = pHttpFile->Read(szResponse, 4096);

            szResponse[nRead] = 0;

            strResponse += szResponse;

            dwResponseLength = pHttpFile->GetLength();
        }
    }
    catch (CInternetException* e)
    {
        TCHAR szError[255];

        e->GetErrorMessage(szError,255);
        e->Delete();

        TRACE(_T("Error : '%s' \n"), CString(szError));

        goto clearVariabls;
    }

    strResponse.Replace(_T("\n"), _T("\r\n"));

    bRetValue = TRUE;

    //  TRACE(_T("strResponse = '%s'\n"), strResponse);

clearVariabls:
    if (pHttpFile)
        pHttpFile->Close();

    if (pHttpConn)
        pHttpConn->Close();

    HSP_SAFE_DELETE(pHttpFile);
    HSP_SAFE_DELETE(pHttpConn);

    oSession.Close();

    return bRetValue;
}

BOOL ccWin32WebApiHelper::DoMakeRequestAPI(CString& strAPI, const CString& strFunction, Json::Value& oParams)
{
    Json::Value oAPI;

    oAPI["method"]    = (char*)CT2A(strFunction);
    oAPI["jsonrpc"]   = "2.0";
    oAPI["id"]        = 1;

    if (_strToken.GetLength() > 0)
        oAPI["token"] = (LPCTSTR)_strToken;

    oParams["need_key_type"] = false;

    oAPI["params"]    = oParams;

    Json::StyledWriter write;
    strAPI = write.write(oAPI).c_str();

    return TRUE;
}

void ccWin32WebApiHelper::OnHSTimer(EtDWORD uIDEvent)
{
    switch (uIDEvent)
    {
    case TID_CHECK_TRANSACTION:
        if (_aTransactionInfos.Size() > 0)
        {
            CString                 strResponse;
            sp<XTransactionInfo>    pWorkTransction;

            _mtx.Lock();
            pWorkTransction = _aTransactionInfos[0];
            _aTransactionInfos.RemoveAt(0);
            _mtx.Unlock();

            if (pWorkTransction->_pNotifier)
            {
                if (DoSendRequestAPI(pWorkTransction->_strAPI, strResponse))
                    pWorkTransction->_pNotifier->OnTransactionRecveResponse(strResponse);
                else
                    pWorkTransction->_pNotifier->OnTransactionRequestTimeout();
            }
        }
        break;
    }
}


BOOL ccWin32WebApiHelper::getValueString(Json::Value& oValue, const char* strName, CString& strValue, CString strDefaultValue)
{
    strValue = strDefaultValue;

    if (oValue.isMember(strName))
    {
        if (oValue[strName].isString())
        {
            strValue = oValue[strName].asCString();

            return TRUE;
        }
    }

    return NULL;
}

int ccWin32WebApiHelper::getValueInt(Json::Value& oValue, const char* strName, int& nValue, int nDefaultValue)
{
    nValue = nDefaultValue;

    if (oValue.isMember(strName))
    {
        if (oValue[strName].isInt())
        {
            nValue = oValue[strName].asInt();

            return TRUE;
        }
    }

    return FALSE;
}

/* ====================================================================
 *  File: ccWin32WebApiHelper.cpp
 *
 *  Desc:
 *      Win32±â¹Ý Multimedia Timer
 *
 *  Author:
 *      ±è¸¸¼ö(kmansoo@humaxdigital.com)
 *
 *  Date:
 *      2014.11.29
 */
