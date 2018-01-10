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

class ccMongooseWebsocket : public ccWebsocket {
public:
    ccMongooseWebsocket(const std::string& uri, struct mg_connection* con);
    ccMongooseWebsocket(const char* uri, std::size_t size, struct mg_connection* connection);

    virtual ~ccMongooseWebsocket();

public:
    virtual bool            open(const std::string& uri);
    virtual bool            close();
    virtual std::int32_t    get_instance();  // It may be a Socket ID. 
    virtual void*           get_connection_info();  
    virtual const std::string get_peer_ip();

    virtual bool            send(const std::string& message);
    virtual bool            send_binary(const void* buffer, std::size_t size);

private:
    struct mg_connection*   mg_connection_;
    int                     socket_id_;
};

}

#endif /* THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERRESPONSE_H_ */
