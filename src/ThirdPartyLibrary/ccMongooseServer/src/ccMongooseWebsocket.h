/*
 * ccMongooseWebsocket.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#ifndef THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSOCKET_H_
#define THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSOCKET_H_

#include "ccWebServer/ccWebsocket.h"

#include "mongoose/mongoose.h"

namespace Luna {

class ccMongooseWebsocket : public ccWebsocket
{
public:
    ccMongooseWebsocket(const std::string& strUri, struct mg_connection* con);
    ccMongooseWebsocket(const char* pUri, std::size_t size, struct mg_connection* con);

    virtual ~ccMongooseWebsocket();

public:
    virtual bool            open(const std::string& strUri);
    virtual bool            close();
    virtual std::int32_t    getInstance();  // It may be a Socket ID. 
    virtual bool            send(const std::string& strMessage);
    virtual bool            sendBinary(const void* pBuffer, std::size_t size);

private:
    struct mg_connection*   _pMgConnection;
};

}

#endif /* THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERRESPONSE_H_ */
