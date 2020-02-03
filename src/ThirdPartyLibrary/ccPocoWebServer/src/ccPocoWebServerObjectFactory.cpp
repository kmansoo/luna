#include "ccPocoWebServer.h"

#include "ccPocoWebServer/ccPocoWebServerObjectFactory.h"

namespace Luna {

ccPocoWebServerObjectFactory::ccPocoWebServerObjectFactory() {
    // TODO Auto-generated constructor stub
}

ccPocoWebServerObjectFactory::~ccPocoWebServerObjectFactory() {
    // TODO Auto-generated destructor stub
}

std::shared_ptr<ccWebServer>
ccPocoWebServerObjectFactory::create_web_server(
    const std::string& name,
    const std::string& ports,
    const std::string& root_path,
    std::shared_ptr<ccWebServerPageDirectory> page_directory) {

    return std::make_shared<ccPocoWebServer>(name, ports, root_path, page_directory);
}

}
