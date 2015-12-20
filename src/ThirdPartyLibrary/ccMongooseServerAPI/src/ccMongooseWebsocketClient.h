/*
 * ccMongooseWebsocketClient.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#ifndef THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_ccMongooseWebsocketClient_CLIENT_H_
#define THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_ccMongooseWebsocketClient_CLIENT_H_

#include "ccWebServerAPI/ccWebsocketClient.h"

#include "mongoose/mongoose.h"

class ccMongooseWebsocketClient : public ccWebsocketClient
{
public:
    ccMongooseWebsocketClient();
    virtual ~ccMongooseWebsocketClient();
};

#endif /* THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERRESPONSE_H_ */
