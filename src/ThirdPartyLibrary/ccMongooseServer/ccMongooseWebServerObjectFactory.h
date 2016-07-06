/*
 * ccMongooseWebServerObjectFactory.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#ifndef THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVEROBJECTFACTORY_H_
#define THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVEROBJECTFACTORY_H_

#include "ccWebServer/ccWebServerObjectFactory.h"

namespace Luna {

class ccMongooseWebServerObjectFactory : public ccWebServerObjectFactory
{
public:
    ccMongooseWebServerObjectFactory();
    virtual ~ccMongooseWebServerObjectFactory();

public:
    virtual  std::shared_ptr<ccWebServer> CreateWebServer(const std::string& name, const std::string& ports, const std::string& root_path);
};

}

#endif /* THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVEROBJECTFACTORY_H_ */
