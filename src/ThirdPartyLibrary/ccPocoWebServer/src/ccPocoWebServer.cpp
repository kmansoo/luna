#include <functional>

#include "ccCore/ccString.h"

#include "ccPocoWebServer.h"
#include "ccPocoWebServerRequest.h"
#include "ccPocoWebServerResponse.h"
#include "ccPocoWebsocket.h"

#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/AbstractHTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPResponse.h"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServerParams;

using Poco::Net::HTTPMessage;

using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPRequestHandler;

namespace Luna {

class ccPocoRequestHandler: public HTTPRequestHandler
{
public:
  ccPocoRequestHandler(ccWebServerEventListener* server_event_listener) : server_event_listener_(server_event_listener) {}

public:
  virtual void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
  {
    if (server_event_listener_ != nullptr) {
      auto new_request = std::make_shared<ccPocoWebServerRequest>(request);
      auto new_response = std::make_shared<ccPocoWebServerResponse>(response);

      server_event_listener_->on_web_request(new_request, new_response);
    }
  }

private:
  ccWebServerEventListener* server_event_listener_ = nullptr;
};


class ccPocoRequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
  ccPocoRequestHandlerFactory(ccWebServerEventListener* server_event_listener)
  {
    main_request_handler_ = std::make_shared<ccPocoRequestHandler>(server_event_listener);
  }

public:
  HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
  {
    return main_request_handler_.get();
  }

private:
  std::shared_ptr<HTTPRequestHandler> main_request_handler_;
};

ccPocoWebServer::ccPocoWebServer(const std::string& name, const std::string& ports, const std::string& root_path, std::shared_ptr<ccWebServerPageDirectory> page_directory)
    : ccWebServer(name, ports, root_path, page_directory)
{
}

ccPocoWebServer::~ccPocoWebServer()
{
  stop();
}

void ccPocoWebServer::set_upload_event(const std::string& path)
{
}

bool ccPocoWebServer::start()
{
  if (web_server_ != nullptr) {
    return false;
  }

  //  server_socket_ = std::make_shared<ServerSocket>(8091);
  HTTPServerParams* server_params = new HTTPServerParams;
  server_params->setKeepAlive(false);

  web_server_ = std::make_shared<HTTPServer>(
                  new ccPocoRequestHandlerFactory(this->eventListener_), 
                  std::stoi(httpPorts_), 
                  server_params);

  web_server_->start();

  return true;
}

bool ccPocoWebServer::stop()
{
  if (web_server_ == nullptr) {
    return false;
  }

  web_server_->stop();

  return true;
}
}
