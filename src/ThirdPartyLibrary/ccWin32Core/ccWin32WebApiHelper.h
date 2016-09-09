#pragma once

#include <memory>
#include <mutex>
#include <thread>
#include <functional>

#include "ccJsonParser/json/json.h"
#include "ccWebServer/ccWebServerRequest.h"
#include "ccWin32RgWebApiTransactionNotifier.h"

namespace Luna {

class ccWin32WebApiHelper {
protected:
    ccWin32WebApiHelper(void);

public:
    virtual ~ccWin32WebApiHelper(void);

public:
    static std::shared_ptr<ccWin32WebApiHelper> get_instance();

public:
    void setConnectionInfo(const std::string& strIP, UINT uPort);

    std::uint16_t request_api(ccWebServerRequest::HttpMethod method,
                              const std::string& web_api,
                              std::string& response);
    std::uint16_t request_api(ccWebServerRequest::HttpMethod method,
                              const std::string& web_api,
                              Json::Value& params,
                              std::string& response);

    bool async_request_api(ccWebServerRequest::HttpMethod method,
                           const std::string& web_api,
                           ccWin32RgWebApiTransactionNotifier* notifier = NULL);
    bool async_request_api(ccWebServerRequest::HttpMethod method,
                           const std::string& web_api,
                           Json::Value& params,
                           ccWin32RgWebApiTransactionNotifier* notifier = NULL);

public:
    static bool get_value_string(Json::Value& json_value,
                                 const std::string& name,
                                 std::string& value,
                                 std::string default_value = _T(""));

    static bool get_value_int(Json::Value& json_value,
                              const std::string& name,
                              int& nValue,
                              int default_value = 0);

private:
    std::uint16_t send_request_api(ccWebServerRequest::HttpMethod method,
                                   const std::string& web_api,
                                   const std::string& request,
                                   std::string& response);
    void run_thread();

private:
    class XTransactionInfo {
    public:
        XTransactionInfo() { notifier_ = NULL; }
        virtual ~XTransactionInfo() {}

    public:
        ccWebServerRequest::HttpMethod method_;
        std::string web_api_;
        std::string request_data_;

        ccWin32RgWebApiTransactionNotifier* notifier_;
    };

private:
    std::string dest_ip_;
    UINT dest_port_;
    std::mutex mutex_;

    std::thread polling_thread_;
    bool is_stop_thread_;

    std::vector<std::shared_ptr<XTransactionInfo>> transaction_info_list_;
};

}
