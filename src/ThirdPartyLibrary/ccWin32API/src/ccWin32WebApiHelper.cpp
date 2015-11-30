#include "stdafx.h"

#include "afxinet.h"
#include "ccWin32WebApiHelper.h"

ccWin32WebApiHelper::ccWin32WebApiHelper(void) :
    _strDestIP(_T("192.168.0.1")),
    _uDestPort(80),
    _bIsStopThread(false),
    _pPollThread(NULL)
{
    _bIsStopThread = false;

    _pPollThread = new std::thread(std::bind(&ccWin32WebApiHelper::DoRunThread, this));
}

ccWin32WebApiHelper::~ccWin32WebApiHelper(void)
{
    if (_pPollThread)
    {
        _bIsStopThread = true;

        _pPollThread->join();

        delete _pPollThread;

        _pPollThread = NULL;
    }
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

void ccWin32WebApiHelper::SetConnectionInfo(const std::string& strIP, UINT uPort)
{
    _strDestIP  =   strIP;
    _uDestPort  =   uPort;
}

std::uint16_t ccWin32WebApiHelper::RequestAPI(ccWebServerRequest::HttpMethod eMethod, const std::string& strWebAPI, std::string& strResponse)
{
    return DoSendRequestAPI(eMethod, strWebAPI, "", strResponse);
}

std::uint16_t ccWin32WebApiHelper::RequestAPI(ccWebServerRequest::HttpMethod eMethod, const std::string& strWebAPI, Json::Value& oParams, std::string& strResponse)
{
    Json::StyledWriter writer;

    return DoSendRequestAPI(eMethod, strWebAPI, writer.write(oParams), strResponse);
}

bool ccWin32WebApiHelper::AsyncRequestAPI(ccWebServerRequest::HttpMethod eMethod, const std::string& strWebAPI, ccWin32RgWebApiTransactionNotifier *pNotifier)
{
    Json::Value oParams;

    return AsyncRequestAPI(eMethod, strWebAPI, oParams, pNotifier);
}

bool ccWin32WebApiHelper::AsyncRequestAPI(ccWebServerRequest::HttpMethod eMethod, const std::string& strWebAPI, Json::Value& oParams, ccWin32RgWebApiTransactionNotifier *pNotifier)
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

std::uint16_t ccWin32WebApiHelper::DoSendRequestAPI(ccWebServerRequest::HttpMethod eMethod, const std::string& strWebAPI, const std::string& strRequestData, std::string& strResponse)
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

    sReqHdr.Format(_T("Content-Type: application/javascript"));  
    pHttpFile->AddRequestHeaders(sReqHdr);  

    //  Request
    strResultString = _T("");

    BOOL bRet = false;
    char szResponse[5000];  

    try {

        bRet = pHttpFile->SendRequestEx(strRequestData.length(), HSR_ASYNC | HSR_INITIATE);

        pHttpFile->Write(CT2A(strRequestData.c_str()), strRequestData.length());

        bRet = pHttpFile->EndRequest(HSR_ASYNC);  

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

//bool ccWin32WebApiHelper::DoMakeRequestAPI(std::string& strAPI, const std::string& strFunction, Json::Value& oParams)
//{
//    Json::Value oAPI;
//
//    oAPI["jsonrpc"]   = "2.0";
//    oAPI["id"]        = 1;
//
//    oAPI["params"]    = oParams;
//
//    Json::StyledWriter write;
//    strAPI = write.write(oAPI).c_str();
//
//    return true;
//}

void ccWin32WebApiHelper::DoRunThread()
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
                std::uint16_t uStatusCode = DoSendRequestAPI(pWorkTransction->_eMethod, pWorkTransction->_strWebAPI, pWorkTransction->_strRequestData, strResponse);

                if (uStatusCode == 0)
                    pWorkTransction->_pNotifier->OnTransactionRecveResponse(uStatusCode, strResponse);
                else
                    pWorkTransction->_pNotifier->OnTransactionRequestTimeout();
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


bool ccWin32WebApiHelper::getValueString(Json::Value& oValue, const std::string& strName, std::string& strValue, std::string strDefaultValue)
{
    strValue = strDefaultValue;

    if (oValue.isMember(strName))
    {
        if (oValue[strName].isString())
        {
            strValue = oValue[strName].asCString();

            return true;
        }
    }

    return false;
}

bool ccWin32WebApiHelper::getValueInt(Json::Value& oValue, const std::string& strName, int& nValue, int nDefaultValue)
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

