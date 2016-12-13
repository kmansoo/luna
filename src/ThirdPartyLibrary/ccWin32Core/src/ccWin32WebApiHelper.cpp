#include "stdafx.h"

#include "afxinet.h"
#include "ccWin32Core/ccWin32WebApiHelper.h"

namespace Luna {

ccWin32WebApiHelper::ccWin32WebApiHelper(void)
    : dest_ip_(_T("192.168.0.1")), dest_port_(80), is_stop_thread_(false) {
    is_stop_thread_ = false;

    polling_thread_ =
        std::thread(std::bind(&ccWin32WebApiHelper::run_thread, this));
}

ccWin32WebApiHelper::~ccWin32WebApiHelper(void) {
    is_stop_thread_ = true;
    polling_thread_.join();
}

std::shared_ptr<ccWin32WebApiHelper> ccWin32WebApiHelper::get_instance() {
    static std::shared_ptr<ccWin32WebApiHelper> s_singleInstance;

    if (s_singleInstance == NULL) {
        std::shared_ptr<ccWin32WebApiHelper> new_instance(new ccWin32WebApiHelper);

        s_singleInstance = new_instance;
    }

    return s_singleInstance;
}

void ccWin32WebApiHelper::setConnectionInfo(const std::string& strIP,
                                            UINT uPort) {
    dest_ip_ = strIP;
    dest_port_ = uPort;
}

std::uint16_t ccWin32WebApiHelper::request_api(
    ccWebServerRequest::HttpMethod method,
    const std::string& web_api,
    std::string& response) {
    return send_request_api(method, web_api, "", response);
}

std::uint16_t ccWin32WebApiHelper::request_api(
    ccWebServerRequest::HttpMethod method,
    const std::string& web_api,
    Json::Value& params,
    std::string& response) {
    Json::StyledWriter writer;

    return send_request_api(method, web_api, writer.write(params), response);
}

bool ccWin32WebApiHelper::async_request_api(
    ccWebServerRequest::HttpMethod method,
    const std::string& web_api,
    ccWin32RgWebApiTransactionNotifier* notifier) {
    Json::Value params;

    return async_request_api(method, web_api, params, notifier);
}

bool ccWin32WebApiHelper::async_request_api(
    ccWebServerRequest::HttpMethod method,
    const std::string& web_api,
    Json::Value& params,
    ccWin32RgWebApiTransactionNotifier* notifier) {
    if (notifier == NULL)
        return false;

    std::shared_ptr<XTransactionInfo> new_transaction(
        std::make_shared<XTransactionInfo>());

    new_transaction->method_ = method;
    new_transaction->notifier_ = notifier;

    new_transaction->web_api_ = web_api;

    Json::StyledWriter writer;
    new_transaction->request_data_ = writer.write(params);

    mutex_.lock();

    bool bExist = false;

    for (auto tr : transaction_info_list_) {
        if (tr->notifier_ == notifier) {
            bExist = true;
            break;
        }
    }

    if (!bExist)
        transaction_info_list_.push_back(new_transaction);

    mutex_.unlock();

    return true;
}

std::uint16_t ccWin32WebApiHelper::send_request_api(
    ccWebServerRequest::HttpMethod method,
    const std::string& web_api,
    const std::string& request,
    std::string& response) {
    USES_CONVERSION;

    std::uint16_t status_code = 0;

    CInternetSession session(_T("MySession"));

    CHttpFile* http_file = NULL;
    CHttpConnection* http_connection = NULL;
    CString uri = _T("");
    DWORD svc_type = 0;
    DWORD option_flags = INTERNET_FLAG_EXISTING_CONNECT;

    uri.Format(__T("http://%s:%d%s"), dest_ip_.c_str(), dest_port_,
               web_api.c_str());

    CString send_data;
    CString server;
    CString object;
    CString request_header;
    CString request_data;

    INTERNET_PORT port = 0;

    if (AfxParseURL(uri, svc_type, server, object, port) == false)
        return status_code;

    try {
        session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 5000);
        http_connection = session.GetHttpConnection(server, port);

        if (http_connection == NULL)
            goto clearVariabls;

        switch (method) {
        case ccWebServerRequest::HttpMethod_Get:
            http_file =
                http_connection->OpenRequest(CHttpConnection::HTTP_VERB_GET, object,
                                             NULL, 1, NULL, NULL, option_flags);
            break;

        case ccWebServerRequest::HttpMethod_Post:
            http_file = http_connection->OpenRequest(
                CHttpConnection::HTTP_VERB_POST, object, NULL, 1, NULL, NULL,
                option_flags);
            break;

        case ccWebServerRequest::HttpMethod_Put:
            http_file =
                http_connection->OpenRequest(CHttpConnection::HTTP_VERB_PUT, object,
                                             NULL, 1, NULL, NULL, option_flags);
            break;

        case ccWebServerRequest::HttpMethod_Delete:
            http_file = http_connection->OpenRequest(
                CHttpConnection::HTTP_VERB_DELETE, object, NULL, 1, NULL, NULL,
                option_flags);
            break;
        }

        if (http_file == NULL)
            goto clearVariabls;

        http_file->SetReadBufferSize(4096);
    } catch (CInternetException* e) {
        TCHAR error_buffer[255];

        e->GetErrorMessage(error_buffer, 255);
        e->Delete();

        goto clearVariabls;
    }

    //  Request
    request_data = _T("");

    BOOL ret_value = false;
    char response_buffer[5000];

    try {
        if (request.size() > 0) {
            request_header.Format(_T("Content-Type: application/javascript"));
            http_file->AddRequestHeaders(request_header);

            ret_value = http_file->SendRequestEx(request.length(),
                                                 HSR_ASYNC | HSR_INITIATE);

            http_file->Write(CT2A(request.c_str()), request.length());

            ret_value = http_file->EndRequest(HSR_ASYNC);
        } else {
            CString strHeaders =
                _T("Content-Type: application/x-www-form-urlencoded");
            ret_value = http_file->SendRequest(strHeaders);
        }

        DWORD result;
        http_file->QueryInfoStatusCode(result);

        status_code = (std::uint16_t)result;

        ULONGLONG dwResponseLength = http_file->GetLength();

        while (0 != dwResponseLength) {
            int nRead = http_file->Read(response_buffer, 4096);

            response_buffer[nRead] = 0;

            request_data += response_buffer;

            dwResponseLength = http_file->GetLength();
        }
    } catch (CInternetException* e) {
        TCHAR error_buffer[255];

        e->GetErrorMessage(error_buffer, 255);
        e->Delete();

        TRACE(_T("Error : '%s' \n"), CString(error_buffer));

        goto clearVariabls;
    }

    request_data.Replace(_T("\n"), _T("\r\n"));

    response = request_data;

    //  TRACE(_T("response = '%s'\n"), response);

clearVariabls:
    if (http_file)
        http_file->Close();

    if (http_connection)
        http_connection->Close();

    if (http_file)
        delete http_file;

    if (http_connection)
        delete http_connection;

    session.Close();

    return status_code;
}

void ccWin32WebApiHelper::run_thread() {
    while (is_stop_thread_ == false) {
        if (transaction_info_list_.size() > 0) {
            std::string response;
            std::shared_ptr<XTransactionInfo> work_transaction;

            mutex_.lock();
            work_transaction = transaction_info_list_[0];
            transaction_info_list_.erase(transaction_info_list_.begin());
            mutex_.unlock();

            if (work_transaction->notifier_) {
                std::uint16_t status_code = send_request_api(
                    work_transaction->method_, work_transaction->web_api_,
                    work_transaction->request_data_, response);

                if (status_code == 0)
                    work_transaction->notifier_->onTransactionRecveResponse(status_code,
                                                                            response);
                else
                    work_transaction->notifier_->onTransactionRequestTimeout();
            }
        }

        Sleep(100);
    }
}

bool ccWin32WebApiHelper::get_value_string(Json::Value& json_value,
                                           const std::string& name,
                                           std::string& value,
                                           std::string default_value) {
    value = default_value;

    if (json_value.isMember(name)) {
        if (json_value[name].isString()) {
            value = json_value[name].asString();

            return true;
        }
    }

    return false;
}

bool ccWin32WebApiHelper::get_value_int(Json::Value& json_value,
                                        const std::string& name,
                                        int& nValue,
                                        int default_value) {
    nValue = default_value;

    if (json_value.isMember(name)) {
        if (json_value[name].isInt()) {
            nValue = json_value[name].asInt();

            return true;
        }
    }

    return false;
}
}
