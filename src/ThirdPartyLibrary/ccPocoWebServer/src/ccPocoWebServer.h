#pragma once

#include <memory>

#include "ccWebServer/ccWebServer.h"

#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/HTTPServer.h"

using Poco::Net::ServerSocket;
using Poco::Net::HTTPServer;

namespace Luna {

class ccPocoWebServer : public ccWebServer {
public:
  ccPocoWebServer(const std::string& name, const std::string& ports, const std::string& root_path, std::shared_ptr<ccWebServerPageDirectory> page_directory = nullptr);
  virtual ~ccPocoWebServer();

public:
  virtual bool start();
  virtual bool stop();
  virtual void set_upload_event(const std::string& path);

private:
  std::shared_ptr<ServerSocket> server_socket_;
  std::shared_ptr<HTTPServer> web_server_;

  std::string last_multipart_request_uri_;

};
}
