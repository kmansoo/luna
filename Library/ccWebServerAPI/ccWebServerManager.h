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
#include "ccRESTfulApi.h"

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

    bool    AddRESTfulApi(shared_ptr<ccRESTfulApi> pNewAPI);
    bool    RemoveRESTfulApi(shared_ptr<ccRESTfulApi> pNewAPI);
    bool    RemoveAllRESTfulApi();

protected:
    virtual bool    OnWebServerRequest(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response);

private:
    std::shared_ptr<ccWebServerObjectFactory>   _pObjFactory;
    vector< shared_ptr<ccWebServer> >           _aWebServers;
    vector< shared_ptr<ccRESTfulApi> >          _aWebAPIs;
};

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERMANAGER_H_ */
