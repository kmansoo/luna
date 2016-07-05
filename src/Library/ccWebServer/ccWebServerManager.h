/*
 * ccWebServerManager.h
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERMANAGER_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERMANAGER_H_

#include <memory>

#include "ccCore/ccSingletonT.h"

#include "ccWebServer.h"
#include "ccWebServerObjectFactory.h"
#include "ccWebServerEventListener.h"

#include "ccRESTfulApi.h"
#include "ccWebsocketManager.h"

class ccWebServerManager : 
    public ccSingletonT<ccWebServerManager>,
    public ccWebServerEventListener
{
public:
    virtual ~ccWebServerManager();

protected:
    ccWebServerManager();
    friend class ccSingletonT<ccWebServerManager>;

public:
    void    AttachFactory(std::shared_ptr<ccWebServerObjectFactory> pFactory);

    bool    CreateWebServer(const std::string& name, const std::string& ports);
    bool    CreateWebServer(const std::string& name, const std::string& ports, const std::string& root_path);

    std::shared_ptr<ccWebServer> GetWebServer(const char* strName);

    void    Start();
    void    Stop();

    bool    AddRESTfulApi(std::shared_ptr<ccRESTfulApi> pNewAPI);
    bool    RemoveRESTfulApi(std::shared_ptr<ccRESTfulApi> pNewAPI);
    bool    RemoveAllRESTfulApi();

    bool    AddWebsocketManager(std::shared_ptr<ccWebsocketManager> pNewWSGM);
    bool    RemoveWebsocketManager(std::shared_ptr<ccWebsocketManager> pNewWSGM);
    bool    RemoveAllWebsocketManager();

protected:
    virtual bool    OnWebServerRequest(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response);

    virtual bool    OnNewWebsocketRequest(const std::string& strWebsocketUri);
    virtual void    OnWebsocketCreated(std::shared_ptr<ccWebsocket> newWebsocket);
    virtual void    OnWebsocketConnected(std::int32_t socketID);
    virtual void    OnWebsocketReceivedData(std::int32_t socketID, const std::string& strData);
    virtual void    OnWebsocketDisconnected(std::int32_t socketID);

protected:
    void    DoPerformWebsocketEvent(ccWebsocket::ccWebSocketEvent eEvent, std::int32_t socketID, const std::string& strData);

private:
    std::shared_ptr<ccWebServerObjectFactory>       _pObjFactory;
    std::vector< std::shared_ptr<ccWebServer> >     _aWebServers;

    std::vector< std::shared_ptr<ccRESTfulApi> >        _aWebAPIs;
    std::vector<std::shared_ptr<ccWebsocketManager> >   _aWSMs;

    const std::string                                   _strNoData;
};

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERMANAGER_H_ */
