/*
 * ccMongooseWebsocket.cpp
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#include "ccMongooseWebsocket.h"

ccMongooseWebsocket::ccMongooseWebsocket(const std::string& strUri, struct mg_connection* con) :
    ccWebsocket(strUri),
    _pMgConnection(con)
{
    // TODO Auto-generated constructor stub

}

ccMongooseWebsocket::ccMongooseWebsocket(const char* pUri, std::size_t size, struct mg_connection* con) :
    ccWebsocket(pUri, size),
    _pMgConnection(con)
{
    // TODO Auto-generated constructor stub

}

ccMongooseWebsocket::~ccMongooseWebsocket()
{
    // TODO Auto-generated destructor stub
}

std::int32_t ccMongooseWebsocket::GetInstance()  // It may be a Socket ID. 
{
    if (_pMgConnection == NULL)
        return -1;

    return _pMgConnection->sock;
}

bool ccMongooseWebsocket::Send(const char* strMessage, std::size_t size)
{
    if (_pMgConnection == NULL)
        return false;

    mg_send_websocket_frame(_pMgConnection, WEBSOCKET_OP_TEXT, strMessage, size);

    return true;
}
