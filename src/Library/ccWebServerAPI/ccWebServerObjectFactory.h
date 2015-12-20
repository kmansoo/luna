/*
 * ccWebServerObjectFactory.h
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVEROBJECTFACTORY_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVEROBJECTFACTORY_H_

#include <memory>

#include "ccWebServer.h"

class ccWebServerObjectFactory
{
public:
    virtual std::shared_ptr<ccWebServer>        CreateWebServer(const char* name, const char* ports) = 0;
};

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVEROBJECTFACTORY_H_ */
