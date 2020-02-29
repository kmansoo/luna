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
  ccPocoRequestHandler(ccWebServerEventListener* server_event_listener, bool enable_cors = false) 
    : server_event_listener_(server_event_listener),
      enable_cors_(enable_cors)
  {
  }

  virtual ~ccPocoRequestHandler() {
  }

private:
  void respond(HTTPServerResponse& response, uint32_t code, const std::string& reason = "") 
  {
    response.setStatus((HTTPResponse::HTTPStatus)code);

    if (reason.length() > 0) {
      response.setReason(reason);
    }

    response.setContentLength(0);
    response.send();
  }

public:
  virtual void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
  {
    // CORS
    if (enable_cors_ == true) {
        try {
        if (request.has("Origin") == true) {
          response.set("Access-Control-Allow-Origin", request.get("Origin"));
        }
        else {
          if (request.has("origin") == true) {
            response.set("Access-Control-Allow-Origin", request.get("origin"));
          }
        }

        //  response.set("Access-Control-Allow-Origin", "*");
        response.set("Access-Control-Allow-Credentials", "true");
        response.set("Access-Control-Allow-Methods", "GET, PUT, POST, UPDATE, OPTIONS, DELETE");
        response.set("Access-Control-Max-Age", "3600");
      }
			catch (Poco::Exception& exc)
			{
        std::cout << __FUNCTION__ << ": ERROR: " << exc.displayText() << std::endl;
        respond(response, 400);
        return;
			}
    }
    
    if (request.getMethod() == "OPTIONS") {
      respond(response, 200);
      return;
    }

    if (server_event_listener_ == nullptr) {
      respond(response, 404);
      return;
    }

    // Check whether this reqeust is WebSocket or not.
    if (request.hasToken("Connection", "upgrade") && Poco::icompare(request.get("Upgrade", ""), "websocket") == 0) {
      std::cout << __FUNCTION__ << ": Request for WebSocket..." << std::endl;

      if (server_event_listener_->on_new_websocket_request(request.getURI()) == false) {
        std::cout << __FUNCTION__ << ": Doesn't support Websocket for " << request.getURI() << std::endl;
        std::cout << __FUNCTION__ << ": Repond with 404." << std::endl;

        respond(response, 404);
        return;
      }

      auto new_websocket = std::make_shared<ccPocoWebsocket>(request.getURI());

      if (new_websocket->init(request, response) == true) {
        ccPocoWebsocketManager::instance().add_web_socket(new_websocket);

        server_event_listener_->on_websocket_created(new_websocket);
        server_event_listener_->on_websocket_connected(new_websocket->get_instance());

        new_websocket->run(server_event_listener_);
      }
      else {
        std::cout << __FUNCTION__ << ": Coudln't init Websocket" << std::endl;
        std::cout << __FUNCTION__ << ": Repond with 404." << std::endl;

        respond(response, 404);
        return;
      }
    }
    else
    {
      auto new_request = std::make_shared<ccPocoWebServerRequest>(request);
      auto new_response = std::make_shared<ccPocoWebServerResponse>(response);

      server_event_listener_->on_web_request(new_request, new_response);
    }
  }

private:
  ccWebServerEventListener* server_event_listener_ = nullptr;
  bool enable_cors_ = false;
};


class ccPocoRequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
  ccPocoRequestHandlerFactory(ccWebServerEventListener* server_event_listener, bool enable_cors = false) 
    : server_event_listener_(server_event_listener), 
      enable_cors_(enable_cors)
  {
    //  main_request_handler_ = std::make_shared<ccPocoRequestHandler>(server_event_listener);
  }

public:
  HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
  {
    return new ccPocoRequestHandler(server_event_listener_, enable_cors_);
  }

private:
  //  std::shared_ptr<HTTPRequestHandler> main_request_handler_;
  ccWebServerEventListener* server_event_listener_;
  bool enable_cors_ = false;
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
                  new ccPocoRequestHandlerFactory(this->eventListener_, enable_cors_), 
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
