/*
 * ccMongooseWebServerObjectFactory.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#ifndef THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVEROBJECTFACTORY_H_
#define THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVEROBJECTFACTORY_H_

#include "ccWebServerAPI/ccWebServerObjectFactory.h"

class ccMongooseWebServerObjectFactory : public ccWebServerObjectFactory
{
public:
    ccMongooseWebServerObjectFactory();
    virtual ~ccMongooseWebServerObjectFactory();

public:
    virtual  std::shared_ptr<ccWebServer> CreateWebServer(const char* name, const char* ports);
};

#endif /* THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVEROBJECTFACTORY_H_ */
