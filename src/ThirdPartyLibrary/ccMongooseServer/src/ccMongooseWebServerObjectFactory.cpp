/*
 * ccMongooseWebServerObjectFactory.cpp
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#include "ccMongooseWebServer.h"

#include "../ccMongooseWebServerObjectFactory.h"

namespace Luna {

ccMongooseWebServerObjectFactory::ccMongooseWebServerObjectFactory()
{
    // TODO Auto-generated constructor stub

}

ccMongooseWebServerObjectFactory::~ccMongooseWebServerObjectFactory()
{
    // TODO Auto-generated destructor stub
}

std::shared_ptr<ccWebServer> ccMongooseWebServerObjectFactory::createWebServer(const std::string& name, const std::string& ports, const std::string& root_path, std::shared_ptr<ccWebServerPageDirectory> page_directory)
{
    return std::make_shared<ccMongooseWebServer>(name, ports, root_path, page_directory);
}

}
