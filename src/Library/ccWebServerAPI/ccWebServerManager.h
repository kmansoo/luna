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
#include "ccWebsocketManager.h"

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

    bool    AddWebsocketManager(shared_ptr<ccWebsocketManager> pNewWSGM);
    bool    RemoveWebsocketManager(shared_ptr<ccWebsocketManager> pNewWSGM);
    bool    RemoveAllWebsocketManager();

protected:
    virtual bool    OnWebServerRequest(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response);

    virtual bool    OnNewWebsocketRequest(const std::string& strWebsocketUri);
    virtual void    OnWebsocketCreated(std::shared_ptr<ccWebsocket> newWebsocket);
    virtual void    OnWebsocketConnected(std::int32_t socketID);
    virtual void    OnWebsocketData(std::int32_t socketID, const char* pData, std::size_t size);
    virtual void    OnWebsocketClosed(std::int32_t socketID);

protected:
    void    DoPerformWebsocketEvent(ccWebsocket::ccWebSocketEvent eEvent, std::int32_t socketID, const char* pData = NULL, std::size_t size = 0);

private:
    std::shared_ptr<ccWebServerObjectFactory>   _pObjFactory;
    vector< shared_ptr<ccWebServer> >           _aWebServers;

    vector< shared_ptr<ccRESTfulApi> >         _aWebAPIs;
    vector< shared_ptr<ccWebsocketManager> >   _aWSMs;
};

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERMANAGER_H_ */
