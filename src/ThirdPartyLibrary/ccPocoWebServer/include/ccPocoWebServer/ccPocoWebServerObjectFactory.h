#pragma once

#include "ccWebServer/ccWebServerObjectFactory.h"

namespace Luna {

class ccPocoWebServerObjectFactory : public ccWebServerObjectFactory {
public:
  ccPocoWebServerObjectFactory();
  virtual ~ccPocoWebServerObjectFactory();

public:
  virtual std::shared_ptr<ccWebServer> create_web_server(
      const std::string& name,
      const std::string& ports,
      const std::string& root_path,
      std::shared_ptr<ccWebServerPageDirectory> page_directory = nullptr);
};
}
