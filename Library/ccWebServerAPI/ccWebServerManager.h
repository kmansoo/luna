/*
 * ccWebServerManager.h
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERMANAGER_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERMANAGER_H_

#include <memory>

#include "ccWebServer.h"
#include "ccWebServerObjectFactory.h"
#include "ccWebServerEventListener.h"

class ccWebServerManager : public ccWebServerEventListener
{
public:
    ccWebServerManager();
    virtual ~ccWebServerManager();

public:
    void    AttachFactory(std::shared_ptr<ccWebServerObjectFactory> pFactory);

    bool    CreateWebServer(const char* name, const char* ports);
    shared_ptr<ccWebServer> GetWebServer(const char* strName);

    void    Start();
    void    Stop();

protected:
    virtual bool    OnWebServerRequest(ccWebServerRequest* request, ccWebServerResponse* response);

private:
    std::shared_ptr<ccWebServerObjectFactory>   _pObjFactory;
    vector< shared_ptr<ccWebServer> >           _aWebServers;
};

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERMANAGER_H_ */
