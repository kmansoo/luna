
#include "ccBoostWebServer.h"

#include "../ccBoostWebServerObjectFactory.h"

ccBoostWebServerObjectFactory::ccBoostWebServerObjectFactory()
{
}

ccBoostWebServerObjectFactory::~ccBoostWebServerObjectFactory()
{
}

std::shared_ptr<ccWebServer> ccBoostWebServerObjectFactory::CreateWebServer(const char* name, const char* ports)
{
    return std::make_shared<ccBoostWebServer>(name, ports);
}
