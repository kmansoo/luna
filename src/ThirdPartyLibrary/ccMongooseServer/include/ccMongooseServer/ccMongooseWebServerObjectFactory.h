/*
 * ccMongooseWebServerObjectFactory.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#pragma once

#include "ccWebServer/ccWebServerObjectFactory.h"

namespace Luna {

class ccMongooseWebServerObjectFactory : public ccWebServerObjectFactory {
public:
    ccMongooseWebServerObjectFactory();
    virtual ~ccMongooseWebServerObjectFactory();

public:
    virtual std::shared_ptr<ccWebServer> create_web_server(
        const std::string& name,
        const std::string& ports,
        const std::string& root_path,
        std::shared_ptr<ccWebServerPageDirectory> page_directory = nullptr);
};
}
