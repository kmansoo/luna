/*
 * ccMongooseWebServerResponse.cpp
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#include "ccMongooseWebServerResponse.h"

namespace Luna {

ccMongooseWebServerResponse::ccMongooseWebServerResponse(struct mg_connection* con) :
    mg_connection_(con) {
}

ccMongooseWebServerResponse::~ccMongooseWebServerResponse() {
    // TODO Auto-generated destructor stub
}

size_t ccMongooseWebServerResponse::doWriteContentToConnector(const char* strBuf, size_t size) {
    mg_send(mg_connection_, strBuf, size);

    return size;
}

}
