#pragma once

#include <memory>
#include <thread>

#include "ccWebServer/ccWebsocket.h"
#include "ccWebServer/ccWebServerEventListener.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::WebSocket;
using Poco::Net::WebSocketException;

namespace Luna {

class ccPocoWebsocket : public ccWebsocket {
public:
  ccPocoWebsocket(ccWebServerEventListener* server_event_listener, HTTPServerRequest& request, HTTPServerResponse& response);
  virtual ~ccPocoWebsocket();

public:
  virtual bool open(const std::string& uri);
  virtual bool close();
  virtual std::int32_t get_instance(); // It may be a Socket ID.
  virtual void* get_connection_info();
  virtual const std::string get_peer_ip();

  virtual bool send(const std::string& message);
  virtual bool send_binary(const void* buffer, std::size_t size);

private:
  std::thread ws_thread_;

  bool is_ws_thread_stoped_ = false;
  bool is_ws_thread_terminated_ = true;
  
  ccWebServerEventListener* server_event_listener_;

  std::unique_ptr<Poco::Net::WebSocket> web_socket_;
};
}
