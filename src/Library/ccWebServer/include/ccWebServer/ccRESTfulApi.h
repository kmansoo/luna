/*
* ccRESTfulApi.h
*
*  Created on: 2015. 11. 28.
*      Author: kmansoo
*/

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCRESTFULAPI_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCRESTFULAPI_H_

#include <string>
#include <map>
#include <functional>
#include <memory>

#include "ccWebServerRequest.h"
#include "ccWebServerResponse.h"

namespace Luna {

class ccRESTfulApi {
public:
    ccRESTfulApi();
    virtual ~ccRESTfulApi();

public:
    virtual bool has_api(const std::string& uri);
    virtual bool perform_api(
        const std::shared_ptr<ccWebServerRequest> request,
        std::shared_ptr<ccWebServerResponse> response);

public:
    bool addAPI(const std::string& uri,
                std::function<bool(
                    std::shared_ptr<ccWebServerRequest> request,
                    std::shared_ptr<ccWebServerResponse> response)> f);

protected:
    std::map < std::string,
        std::function<bool(
            std::shared_ptr<ccWebServerRequest> request,
            std::shared_ptr<ccWebServerResponse> response) >> api_map_;
};

}

#endif
