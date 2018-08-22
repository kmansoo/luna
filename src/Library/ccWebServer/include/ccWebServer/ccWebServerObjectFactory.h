/*
 * ccWebServerObjectFactory.h
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#pragma once

#include <memory>

#include "ccWebServer.h"

namespace Luna {

class ccWebServerObjectFactory {
public:
    virtual std::shared_ptr<ccWebServer> create_web_server(const std::string& name, const std::string& ports, const std::string& root_path, std::shared_ptr<ccWebServerPageDirectory> page_directory = nullptr) = 0;
};

}
