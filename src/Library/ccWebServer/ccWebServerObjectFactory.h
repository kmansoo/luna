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

namespace Luna {

class ccWebServerObjectFactory
{
public:
    virtual std::shared_ptr<ccWebServer> CreateWebServer(const std::string& name, const std::string& ports, const std::string& root_path) = 0;
};

}

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVEROBJECTFACTORY_H_ */
