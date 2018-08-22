
#pragma once


#include "ccWebServerAPI/ccWebServerObjectFactory.h"

class ccBoostWebServerObjectFactory : public ccWebServerObjectFactory
{
public:
    ccBoostWebServerObjectFactory();
    virtual ~ccBoostWebServerObjectFactory();

public:
    virtual  std::shared_ptr<ccWebServer> CreateWebServer(const char* name, const char* ports);
};
