/*
 * ccRESTfulApi.cpp
 *
 *  Created on: 2015. 11.27
 *      Author: kmansoo
 */

#include "ccWebServer/ccRESTfulApi.h"

namespace Luna {

ccRESTfulApi::ccRESTfulApi() {}

ccRESTfulApi::~ccRESTfulApi() {}

bool ccRESTfulApi::has_api(const std::string& uri) {
    auto it = api_map_.find(uri);

    if (it == api_map_.end()) {
        //  '/xxx/xxx/*' ������ ã��
        std::string tempuri_ = uri;

        int nPos = tempuri_.rfind("/");

        if (nPos >= 0) {
            tempuri_.erase(nPos + 1, (tempuri_.length() - nPos) - 1);

            tempuri_ += "*";
            it = api_map_.find(tempuri_);

            if (it == api_map_.end())
                return false;
        }
    }

    return true;
}

bool ccRESTfulApi::perform_api(
    std::shared_ptr<ccWebServerRequest> request,
    std::shared_ptr<ccWebServerResponse> response) {

    std::string uri = request->geturi_();

    auto it = api_map_.find(uri);

    if (it == api_map_.end()) {
        //  '/xxx/xxx/*' ������ ã��
        std::string tempuri_ = uri;

        int nPos = tempuri_.rfind("/");

        if (nPos >= 0) {
            tempuri_.erase(nPos + 1, (tempuri_.length() - nPos) - 1);

            tempuri_ += "*";
            it = api_map_.find(tempuri_);

            if (it == api_map_.end())
                return false;
        }
    }

    return it->second(request, response);
}

bool ccRESTfulApi::addAPI(
    const std::string& uri,
    std::function<bool(std::shared_ptr<ccWebServerRequest> request,
                       std::shared_ptr<ccWebServerResponse> response)> f) {
    api_map_[uri] = f;

    return true;
}

}
