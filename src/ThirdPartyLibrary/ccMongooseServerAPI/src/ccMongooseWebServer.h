/*
 * ccMongooseWebServer.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#ifndef THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVER_H_
#define THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVER_H_

#include <thread>

#include "mongoose/mongoose.h"

#include "ccWebServerAPI/ccWebServer.h"

class ccMongooseWebServer : public ccWebServer
{
public:
    ccMongooseWebServer(const char* name, const char* ports);
    virtual ~ccMongooseWebServer();

public:
    virtual bool Start();
    virtual bool Stop();

private:
    static void ev_handler(struct mg_connection *nc, int ev, void *p);

private:
    void    DoRunThread();

private:
    bool                    _bIsStopThread;

    std::thread*            _pPollThread;
    struct mg_mgr*          _mgr;
    struct mg_connection*   _con;
};

#endif /* THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVER_H_ */
