/*
 * ccMongooseWebServer.cpp
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#include <functional>

#include "ccCoreAPI/ccString.h"
#include "ccMongooseWebServer.h"

#include "ccMongooseWebServerRequest.h"
#include "ccMongooseWebServerResponse.h"
#include "ccMongooseWebsocket.h"

static struct mg_serve_http_opts s_http_server_opts;

ccMongooseWebServer::ccMongooseWebServer(const std::string& name, const std::string& ports, const std::string& root_path) :
        ccWebServer(name, ports, root_path),
        _bIsStopThread(false),
        _mgr(NULL),
        _con(NULL)
{
}

ccMongooseWebServer::~ccMongooseWebServer()
{
    Stop();

    if (_mgr != NULL)
        delete _mgr;

    _mgr = NULL;
    _con  = NULL;
}


bool ccMongooseWebServer::Start()
{
    if (_mgr != NULL)
        return false;

    _mgr = new struct mg_mgr;

    mg_mgr_init(_mgr, NULL);

    _mgr->user_data = this;
    _con = mg_bind(_mgr, _strHttpPorts.c_str(), ev_handler);

    // Set up HTTP server parameters
    mg_set_protocol_http_websocket(_con);

    s_http_server_opts.document_root = _strWebRootPath.c_str();  // Serve current directory
    s_http_server_opts.enable_directory_listing = "yes";

    printf("Starting web server on port %s\n", _strHttpPorts.c_str());

    _bIsStopThread = false;

    _pPollThread = new std::thread(std::bind(&ccMongooseWebServer::DoRunThread, this));

    return true;
}

bool ccMongooseWebServer::Stop()
{
    if (_pPollThread)
    {
        _bIsStopThread = true;

        _pPollThread->join();

        delete _pPollThread;
    }

    if (_mgr == NULL)
        return false;

    mg_mgr_free(_mgr);

    delete _mgr;

    _mgr    = NULL;
    _con     = NULL;

    return true;
}

void ccMongooseWebServer::DoRunThread()
{
    while (_bIsStopThread == false)
        mg_mgr_poll(_mgr, 200);
}

void ccMongooseWebServer::ev_handler(struct mg_connection *nc, int ev, void *p)
{
    bool bIsBuiltinProcess = true;

    ccMongooseWebServer* pServer = (ccMongooseWebServer*)nc->mgr->user_data;

    /* HTTP and websocket events. void *ev_data is described in a comment. */
    switch (ev)
    {
    case MG_EV_HTTP_REQUEST:    /* struct http_message * */
        if (pServer->_pEventListener != NULL)
        {
            std::shared_ptr<ccMongooseWebServerRequest> pRequest(new ccMongooseWebServerRequest(nc, (http_message *)p));
            std::shared_ptr<ccMongooseWebServerResponse> pResponse(new ccMongooseWebServerResponse(nc));

            bIsBuiltinProcess = !pServer->_pEventListener->OnWebServerRequest(pRequest, pResponse);

            //if (!bIsBuiltinProcess)
            //    pResponse->
        }
        
        if (bIsBuiltinProcess)
            mg_serve_http(nc, (struct http_message *)p, s_http_server_opts);

        break;

    case MG_EV_HTTP_REPLY:      /* struct http_message * */
        break;

    case MG_EV_HTTP_CHUNK:      /* struct http_message * */
        break;

    case MG_EV_SSI_CALL:        /* char * */
        break;

    case MG_EV_WEBSOCKET_HANDSHAKE_REQUEST: /* NULL */
        {
            struct http_message *hm = (struct http_message *)p;

            std::string strRequestUri(hm->uri.p, hm->uri.len);

            if (pServer->_pEventListener->OnNewWebsocketRequest(strRequestUri) == true)
            {
                std::shared_ptr<ccWebsocket> pNewWS(new ccMongooseWebsocket(strRequestUri, nc));

                pServer->_pEventListener->OnWebsocketCreated(pNewWS);
            }
            else
                nc->flags |= MG_F_CLOSE_IMMEDIATELY;
        }
        break;

    case MG_EV_WEBSOCKET_HANDSHAKE_DONE:    /* NULL */
        pServer->_pEventListener->OnWebsocketConnected(nc->sock);
        break;

    case MG_EV_WEBSOCKET_FRAME:             /* struct websocket_message * */
        {
            struct websocket_message *wm = (struct websocket_message *) p;

            pServer->_pEventListener->OnWebsocketData(nc->sock, (const char*)wm->data, wm->size);
        }
        break;

    case MG_EV_WEBSOCKET_CONTROL_FRAME:     /* struct websocket_message * */
        break;

    case MG_EV_CLOSE:
        //  is websocket ?
        if (nc->flags & MG_F_IS_WEBSOCKET)
            pServer->_pEventListener->OnWebsocketClosed(nc->sock);

        break;
    }
}

//
//int main(void) {
//
//  for (;;) {
//    mg_mgr_poll(&mgr, 1000);
//  }
//  mg_mgr_free(&mgr);
//
//  return 0;
//}
