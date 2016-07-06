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

class ccRESTfulApi
{
public:
    ccRESTfulApi();
    virtual ~ccRESTfulApi();

public:
    virtual bool    hasAPI(const std::string& strUri);
    virtual bool    performAPI(const std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse);

public:
    bool    addAPI(const std::string& strUri, std::function<bool(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)> f);

protected:
    std::map < std::string, std::function<bool(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)>> _aAPIs;
};

}

#endif
