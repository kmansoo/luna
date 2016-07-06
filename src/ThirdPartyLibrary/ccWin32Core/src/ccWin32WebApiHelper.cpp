#include "stdafx.h"

#include "afxinet.h"
#include "ccWin32WebApiHelper.h"

namespace Luna {

ccWin32WebApiHelper::ccWin32WebApiHelper(void) :
    _strDestIP(_T("192.168.0.1")),
    _uDestPort(80),
    _bIsStopThread(false)
{
    _bIsStopThread = false;

    _oPollThread = std::thread(std::bind(&ccWin32WebApiHelper::doRunThread, this));
}

ccWin32WebApiHelper::~ccWin32WebApiHelper(void)
{
    _bIsStopThread = true;
    _oPollThread.join();
}

std::shared_ptr<ccWin32WebApiHelper> ccWin32WebApiHelper::getInstance()
{
    static std::shared_ptr<ccWin32WebApiHelper>  s_singleInstance;

    if (s_singleInstance == NULL)
    {
        std::shared_ptr<ccWin32WebApiHelper> newInstance(new ccWin32WebApiHelper);

        s_singleInstance = newInstance;
    }

    return s_singleInstance;
}

void ccWin32WebApiHelper::setConnectionInfo(const std::string& strIP, UINT uPort)
{
    _strDestIP  =   strIP;
    _uDestPort  =   uPort;
}

std::uint16_t ccWin32WebApiHelper::requestAPI(ccWebServerRequest::HttpMethod eMethod, const std::string& strWebAPI, std::string& strResponse)
{
    return doSendRequestAPI(eMethod, strWebAPI, "", strResponse);
}

std::uint16_t ccWin32WebApiHelper::requestAPI(ccWebServerRequest::HttpMethod eMethod, const std::string& strWebAPI, Json::Value& oParams, std::string& strResponse)
{
    Json::StyledWriter writer;

    return doSendRequestAPI(eMethod, strWebAPI, writer.write(oParams), strResponse);
}

bool ccWin32WebApiHelper::asyncRequestAPI(ccWebServerRequest::HttpMethod eMethod, const std::string& strWebAPI, ccWin32RgWebApiTransactionNotifier *pNotifier)
{
    Json::Value oParams;

    return asyncRequestAPI(eMethod, strWebAPI, oParams, pNotifier);
}

bool ccWin32WebApiHelper::asyncRequestAPI(ccWebServerRequest::HttpMethod eMethod, const std::string& strWebAPI, Json::Value& oParams, ccWin32RgWebApiTransactionNotifier *pNotifier)
{
    if (pNotifier == NULL)
        return false;

    std::shared_ptr<XTransactionInfo> pNewTransaction(std::make_shared<XTransactionInfo>());

    pNewTransaction->_eMethod   = eMethod;
    pNewTransaction->_pNotifier = pNotifier;

    pNewTransaction->_strWebAPI         = strWebAPI;

    Json::StyledWriter writer;
    pNewTransaction->_strRequestData = writer.write(oParams);

    _mutex.lock();

    bool bExist = false;

    for (auto tr : _aTransactionInfos)
    {
        if (tr->_pNotifier == pNotifier)
        {
            bExist = true;
            break;
        }
    }

    if (!bExist)
        _aTransactionInfos.push_back(pNewTransaction);

    _mutex.unlock();

    return true;
}

std::uint16_t ccWin32WebApiHelper::doSendRequestAPI(ccWebServerRequest::HttpMethod eMethod, const std::string& strWebAPI, const std::string& strRequestData, std::string& strResponse)
{
    USES_CONVERSION;  

    std::uint16_t       uStatusCode = 0;

    CInternetSession    oSession(_T("MySession"));  

    CHttpFile*          pHttpFile   = NULL;  
    CHttpConnection*    pHttpConn   = NULL;  
    CString             sURL        = _T("");
    DWORD               dwSvcType   = 0;  
    DWORD               dwFlags     = INTERNET_FLAG_EXISTING_CONNECT;

    sURL.Format(__T("http://%s:%d%s"), _strDestIP.c_str(), _uDestPort, strWebAPI.c_str());

    CString             strSendData;
    CString             sServer;
    CString             sObject;
    CString             sReqHdr;
    CString             strResultString;

    INTERNET_PORT       nPort = 0;  

    if (AfxParseURL(sURL, dwSvcType, sServer, sObject, nPort) == false)
        return uStatusCode;

    try {
        oSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 5000);
        pHttpConn = oSession.GetHttpConnection(sServer, nPort);

        if (pHttpConn == NULL)
            goto clearVariabls;

        switch (eMethod)
        {
        case ccWebServerRequest::HttpMethod_Get:
            pHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_GET, sObject, NULL, 1, NULL, NULL, dwFlags);
            break;

        case ccWebServerRequest::HttpMethod_Post:
            pHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_POST, sObject, NULL, 1, NULL, NULL, dwFlags);
            break;
        
        case ccWebServerRequest::HttpMethod_Put:
            pHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_PUT, sObject, NULL, 1, NULL, NULL, dwFlags);
            break;
        
        case ccWebServerRequest::HttpMethod_Delete:
            pHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_DELETE, sObject, NULL, 1, NULL, NULL, dwFlags);
            break;
        }

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


    //  Request
    strResultString = _T("");

    BOOL bRet = false;
    char szResponse[5000];  

    try {
        if (strRequestData.size() > 0)
        {
            sReqHdr.Format(_T("Content-Type: application/javascript"));
            pHttpFile->AddRequestHeaders(sReqHdr);

            bRet = pHttpFile->SendRequestEx(strRequestData.length(), HSR_ASYNC | HSR_INITIATE);

            pHttpFile->Write(CT2A(strRequestData.c_str()), strRequestData.length());

            bRet = pHttpFile->EndRequest(HSR_ASYNC);
        }
        else
        {
            CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
            bRet = pHttpFile->SendRequest(strHeaders);
        }

        DWORD dwRet;
        pHttpFile->QueryInfoStatusCode(dwRet);

        uStatusCode = (std::uint16_t)dwRet;

        ULONGLONG dwResponseLength = pHttpFile->GetLength();  

        while (0 != dwResponseLength)
        {
            int nRead = pHttpFile->Read(szResponse, 4096);

            szResponse[nRead] = 0;

            strResultString += szResponse;

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

    strResultString.Replace(_T("\n"), _T("\r\n"));

    strResponse = strResultString;

    //  TRACE(_T("strResponse = '%s'\n"), strResponse);

clearVariabls:
    if (pHttpFile)
        pHttpFile->Close();

    if (pHttpConn)
        pHttpConn->Close();

    if (pHttpFile)
        delete pHttpFile;

    if (pHttpConn)
        delete pHttpConn;

    oSession.Close();

    return uStatusCode;
}

void ccWin32WebApiHelper::doRunThread()
{
    while (_bIsStopThread == false)
    {
        if (_aTransactionInfos.size() > 0)
        {
            std::string                         strResponse;
            std::shared_ptr<XTransactionInfo>   pWorkTransction;

            _mutex.lock();
            pWorkTransction = _aTransactionInfos[0];
            _aTransactionInfos.erase(_aTransactionInfos.begin());
            _mutex.unlock();

            if (pWorkTransction->_pNotifier)
            {
                std::uint16_t uStatusCode = doSendRequestAPI(pWorkTransction->_eMethod, pWorkTransction->_strWebAPI, pWorkTransction->_strRequestData, strResponse);

                if (uStatusCode == 0)
                    pWorkTransction->_pNotifier->onTransactionRecveResponse(uStatusCode, strResponse);
                else
                    pWorkTransction->_pNotifier->onTransactionRequestTimeout();
            }
        }

        Sleep(100);
    }
}


//void ccWin32WebApiHelper::OnHSTimer(EtDWORD uIDEvent)
//{
//    switch (uIDEvent)
//    {
//    case TID_CHECK_TRANSACTION:
//        if (_aTransactionInfos.size() > 0)
//        {
//            std::string                         strResponse;
//            std::shared_ptr<XTransactionInfo>   pWorkTransction;
//
//            _mutex.lock();
//            pWorkTransction = _aTransactionInfos[0];
//            _aTransactionInfos.erase(_aTransactionInfos.begin());
//            _mutex.unlock();
//
//            if (pWorkTransction->_pNotifier)
//            {
//                if (DoSendRequestAPI(pWorkTransction->_strAPI, strResponse))
//                    pWorkTransction->_pNotifier->OnTransactionRecveResponse(strResponse);
//                else
//                    pWorkTransction->_pNotifier->OnTransactionRequestTimeout();
//            }
//        }
//        break;
//    }
//}


bool ccWin32WebApiHelper::get_value_string(Json::Value& oValue, const std::string& strName, std::string& strValue, std::string strDefaultValue)
{
    strValue = strDefaultValue;

    if (oValue.isMember(strName))
    {
        if (oValue[strName].isString())
        {
            strValue = oValue[strName].asString();

            return true;
        }
    }

    return false;
}

bool ccWin32WebApiHelper::get_value_int(Json::Value& oValue, const std::string& strName, int& nValue, int nDefaultValue)
{
    nValue = nDefaultValue;

    if (oValue.isMember(strName))
    {
        if (oValue[strName].isInt())
        {
            nValue = oValue[strName].asInt();

            return true;
        }
    }

    return false;
}

}
