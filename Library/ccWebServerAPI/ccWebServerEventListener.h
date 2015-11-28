/*
* ccWebServerEventListener.h
*
*  Created on: 2015. 11. 14.
*      Author: kmansoo
*/

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVEREVENTLISTENERH_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVEREVENTLISTENERH_

#include "ccWebServerRequest.h"
#include "ccWebServerResponse.h"

class ccWebServerEventListener
{
public:
    virtual bool OnWebServerRequest(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response) = 0;
};

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVEREVENTLISTENERH_ */
