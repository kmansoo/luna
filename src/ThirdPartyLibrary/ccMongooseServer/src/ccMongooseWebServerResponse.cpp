/*
 * ccMongooseWebServerResponse.cpp
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#include "ccMongooseWebServerResponse.h"

namespace Luna {

ccMongooseWebServerResponse::ccMongooseWebServerResponse(struct mg_connection* con) :
    _pMgConnection(con)
{
    // TODO Auto-generated constructor stub

}

ccMongooseWebServerResponse::~ccMongooseWebServerResponse()
{
    // TODO Auto-generated destructor stub
}

size_t ccMongooseWebServerResponse::DoWriteContentToConnector(const char* strBuf, size_t size)
{
    mg_send(_pMgConnection, strBuf, size);

    return size;
}

}
