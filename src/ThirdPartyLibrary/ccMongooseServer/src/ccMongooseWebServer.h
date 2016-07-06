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

#include "ccWebServer/ccWebServer.h"

namespace Luna {

class ccMongooseWebServer : public ccWebServer
{
public:
    ccMongooseWebServer(const std::string& name, const std::string& ports, const std::string& root_path);
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

}

#endif /* THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVER_H_ */
