/*
 * ccMongooseWebServerObjectFactory.cpp
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#include "ccMongooseWebServer.h"
#include "ccMongooseWebsocketClient.h"

#include "../ccMongooseWebServerObjectFactory.h"

ccMongooseWebServerObjectFactory::ccMongooseWebServerObjectFactory()
{
    // TODO Auto-generated constructor stub

}

ccMongooseWebServerObjectFactory::~ccMongooseWebServerObjectFactory()
{
    // TODO Auto-generated destructor stub
}

std::shared_ptr<ccWebServer> ccMongooseWebServerObjectFactory::CreateWebServer(const char* name, const char* ports)
{
    return std::make_shared<ccMongooseWebServer>(name, ports);
}
