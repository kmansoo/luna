
#ifndef THIRDPARTYLIBRARY_CCBOOSTSERVERAPI_CCBOOSTWEBSERVEROBJECTFACTORY_H_
#define THIRDPARTYLIBRARY_CCBOOSTSERVERAPI_CCBOOSTWEBSERVEROBJECTFACTORY_H_

#include "ccWebServerAPI/ccWebServerObjectFactory.h"

class ccBoostWebServerObjectFactory : public ccWebServerObjectFactory
{
public:
    ccBoostWebServerObjectFactory();
    virtual ~ccBoostWebServerObjectFactory();

public:
    virtual  std::shared_ptr<ccWebServer> CreateWebServer(const char* name, const char* ports);
};

#endif /* THIRDPARTYLIBRARY_CCBOOSTSERVERAPI_CCBOOSTWEBSERVEROBJECTFACTORY_H_ */
