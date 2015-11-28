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
    std::map < string, function<bool(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)>>::iterator it;

    it = _aAPIs.find(strUri);

    if (it == _aAPIs.end())
        return false;

    return true;
}

bool ccRESTfulApi::ExecuteAPI(shared_ptr<ccWebServerRequest> pRequest, shared_ptr<ccWebServerResponse> pResponse)
{
    std::map < string, function<bool(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)>>::iterator it;

    std::string strUri = pRequest->GetURI();

    it = _aAPIs.find(strUri);

    if (it == _aAPIs.end())
        return false;

    return it->second(pRequest, pResponse);
}

bool ccRESTfulApi::AddAPI(std::string& strUri, function<bool(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)> f)
{
    _aAPIs[strUri] = f;

    return true;
}
