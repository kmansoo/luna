/*
 * ccRESTfulApi.cpp
 *
 *  Created on: 2015. 11.27
 *      Author: kmansoo
 */

#include "../ccRESTfulApi.h"

namespace Luna {

ccRESTfulApi::ccRESTfulApi()
{
}


ccRESTfulApi::~ccRESTfulApi()
{
}

bool ccRESTfulApi::hasAPI(const std::string& strUri)
{
    auto it = _aAPIs.find(strUri);

    if (it == _aAPIs.end())
    {
        //  '/xxx/xxx/*' 패턴을 찾음
        std::string strTemp = strUri;

        int nPos = strTemp.rfind("/");

        if (nPos >= 0)
        {
            strTemp.erase(nPos + 1, (strTemp.length() - nPos) - 1);

            strTemp += "*";
            it = _aAPIs.find(strTemp);

            if (it == _aAPIs.end())
                return false;
        }
    }

    return true;
}

bool ccRESTfulApi::performAPI(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)
{
    std::string strUri = pRequest->getURI();

    auto it = _aAPIs.find(strUri);

    if (it == _aAPIs.end())
    {
        //  '/xxx/xxx/*' 패턴을 찾음
        std::string strTemp = strUri;

        int nPos = strTemp.rfind("/");

        if (nPos >= 0)
        {
            strTemp.erase(nPos + 1, (strTemp.length() - nPos) - 1);

            strTemp += "*";
            it = _aAPIs.find(strTemp);

            if (it == _aAPIs.end())
                return false;
        }
    }

    return it->second(pRequest, pResponse);
}

bool ccRESTfulApi::addAPI(const std::string& strUri, std::function<bool(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)> f)
{
    _aAPIs[strUri] = f;

    return true;
}

}
