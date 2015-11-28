/*
 * ccRESTfulChattingApi.h
 *
 *  Created on: 2015. 11. 28.
 *      Author: kmansoo
 */

#ifndef CCPRODUCTLIBRARY_CCRESTFULCHATTINGAPI_CCRESTFULCHATTING_H_
#define CCPRODUCTLIBRARY_CCRESTFULCHATTINGAPI_CCRESTFULCHATTING_H_

#include "ccWebServerAPI/ccRESTfulApi.h"

using namespace std;

class ccRESTfulChatting : public ccRESTfulApi
{
public:
    ccRESTfulChatting();
    virtual ~ccRESTfulChatting();

protected:
    bool    list(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse);
    bool    session(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse);
    bool    session_member(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse);
    bool    session_message(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse);
};


#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVER_H_ */
