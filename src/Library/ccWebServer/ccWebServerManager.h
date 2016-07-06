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

namespace Luna {

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
    void    attachFactory(std::shared_ptr<ccWebServerObjectFactory> pFactory);

    bool    createWebServer(const std::string& name, const std::string& ports);
    bool    createWebServer(const std::string& name, const std::string& ports, const std::string& root_path);

    std::shared_ptr<ccWebServer> getWebServer(const char* strName);

    void    start();
    void    stop();

    bool    addRESTfulApi(std::shared_ptr<ccRESTfulApi> pNewAPI);
    bool    removeRESTfulApi(std::shared_ptr<ccRESTfulApi> pNewAPI);
    bool    removeAllRESTfulApi();

    bool    addWebsocketManager(std::shared_ptr<ccWebsocketManager> pNewWSGM);
    bool    removeWebsocketManager(std::shared_ptr<ccWebsocketManager> pNewWSGM);
    bool    removeAllWebsocketManager();

protected:
    virtual bool    onWebServerRequest(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response);

    virtual bool    onNewWebsocketRequest(const std::string& strWebsocketUri);
    virtual void    onWebsocketCreated(std::shared_ptr<ccWebsocket> newWebsocket);
    virtual void    onWebsocketConnected(std::int32_t socketID);
    virtual void    onWebsocketReceivedData(std::int32_t socketID, const std::string& strData);
    virtual void    onWebsocketDisconnected(std::int32_t socketID);

protected:
    void    doPerformWebsocketEvent(ccWebsocket::ccWebSocketEvent eEvent, std::int32_t socketID, const std::string& strData);

private:
    std::shared_ptr<ccWebServerObjectFactory>       _pObjFactory;
    std::vector< std::shared_ptr<ccWebServer> >     _aWebServers;

    std::vector< std::shared_ptr<ccRESTfulApi> >        _aWebAPIs;
    std::vector<std::shared_ptr<ccWebsocketManager> >   _aWSMs;

    const std::string                                   _strNoData;
};

}

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERMANAGER_H_ */
