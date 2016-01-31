/*
 * ccRESTfulApi.cpp
 *
 *  Created on: 2015. 11.27
 *      Author: kmansoo
 */

#include "../ccRESTfulApi.h"


ccRESTfulApi::ccRESTfulApi()
{
}


ccRESTfulApi::~ccRESTfulApi()
{
}

bool ccRESTfulApi::HasAPI(const std::string& strUri)
{
    auto it = _aAPIs.find(strUri);

    if (it == _aAPIs.end())
        return false;

    return true;
}

bool ccRESTfulApi::PerformAPI(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)
{
    std::string strUri = pRequest->GetURI();

    auto it = _aAPIs.find(strUri);

    if (it == _aAPIs.end())
        return false;

    return it->second(pRequest, pResponse);
}

bool ccRESTfulApi::AddAPI(const std::string& strUri, std::function<bool(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)> f)
{
    _aAPIs[strUri] = f;

    return true;
}
