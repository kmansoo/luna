/*
 * ccMongooseWebServerObjectFactory.cpp
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#include "ccMongooseWebServer.h"

#include "../ccMongooseWebServerObjectFactory.h"

ccMongooseWebServerObjectFactory::ccMongooseWebServerObjectFactory()
{
    // TODO Auto-generated constructor stub

}

ccMongooseWebServerObjectFactory::~ccMongooseWebServerObjectFactory()
{
    // TODO Auto-generated destructor stub
}

std::shared_ptr<ccWebServer> ccMongooseWebServerObjectFactory::CreateWebServer(const std::string& name, const std::string& ports, const std::string& root_path)
{
    return std::make_shared<ccMongooseWebServer>(name, ports, root_path);
}
