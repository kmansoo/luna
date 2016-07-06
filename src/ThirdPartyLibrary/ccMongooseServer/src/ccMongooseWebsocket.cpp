/*
 * ccMongooseWebsocket.cpp
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#include "ccMongooseWebsocket.h"

namespace Luna {

ccMongooseWebsocket::ccMongooseWebsocket(const std::string& strUri, struct mg_connection* con) :
    ccWebsocket(strUri),
    _pMgConnection(con)
{
    // TODO Auto-generated constructor stub

}

ccMongooseWebsocket::~ccMongooseWebsocket()
{
    // TODO Auto-generated destructor stub
}

bool ccMongooseWebsocket::open(const std::string& strUri)
{
    return false;
}

bool ccMongooseWebsocket::close()
{
    return false;
}

std::int32_t ccMongooseWebsocket::getInstance()  // It may be a Socket ID. 
{
    if (_pMgConnection == NULL)
        return -1;

    return _pMgConnection->sock;
}

bool ccMongooseWebsocket::send(const std::string& strMessage)
{
    if (_pMgConnection == NULL)
        return false;

    mg_send_websocket_frame(_pMgConnection, WEBSOCKET_OP_TEXT, strMessage.c_str(), strMessage.length());

    return true;
}

bool ccMongooseWebsocket::sendBinary(const void* pBuffer, std::size_t size)
{
    if (_pMgConnection == NULL)
        return false;

    mg_send_websocket_frame(_pMgConnection, WEBSOCKET_OP_BINARY, pBuffer, size);

    return true;
}

}
