/*
 * ccMongooseWebServerResponse.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#ifndef THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERRESPONSE_H_
#define THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERRESPONSE_H_

#include "ccWebServer/ccWebServerResponse.h"

#include "mongoose/mongoose.h"

namespace Luna {

class ccMongooseWebServerResponse : public ccWebServerResponse
{
public:
    ccMongooseWebServerResponse(struct mg_connection* con);
    virtual ~ccMongooseWebServerResponse();

protected:
    virtual size_t DoWriteContentToConnector(const char* strBuf, size_t size);

private:
    struct mg_connection*   _pMgConnection;
};

}

#endif /* THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERRESPONSE_H_ */
