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

static struct mg_serve_http_opts s_http_server_opts;

ccMongooseWebServer::ccMongooseWebServer(const char* name, const char* ports) :
        ccWebServer(name, ports),
        _bIsStopThread(false),
        _mgr(NULL),
        _nc(NULL)
{
}

ccMongooseWebServer::~ccMongooseWebServer()
{
    Stop();

    if (_mgr != NULL)
        delete _mgr;

    _mgr = NULL;
    _nc  = NULL;
}


bool ccMongooseWebServer::Start()
{
    if (_mgr != NULL)
        return false;

    _mgr = new struct mg_mgr;

    mg_mgr_init(_mgr, NULL);

    _mgr->user_data = this;
    _nc = mg_bind(_mgr, _strHttpPorts.c_str(), ev_handler);

    // Set up HTTP server parameters
    mg_set_protocol_http_websocket(_nc);

    s_http_server_opts.document_root = ".";  // Serve current directory
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
    _nc     = NULL;

    return true;
}

void ccMongooseWebServer::DoRunThread()
{
    while (_bIsStopThread == false)
        mg_mgr_poll(_mgr, 1000);
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
            ccMongooseWebServerRequest  oRequest(nc, (http_message *)p);
            ccMongooseWebServerResponse oResponse(nc);

            bIsBuiltinProcess = !pServer->_pEventListener->OnWebServerRequest(&oRequest, &oResponse);
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
        break;

    case MG_EV_WEBSOCKET_HANDSHAKE_DONE:    /* NULL */
        break;

    case MG_EV_WEBSOCKET_FRAME:             /* struct websocket_message * */
        break;

    case MG_EV_WEBSOCKET_CONTROL_FRAME:     /* struct websocket_message * */
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
