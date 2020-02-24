#pragma once

#include <memory>
#include <thread>

#include "ccCore/ccSingleton.h"

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
  ccPocoWebsocket(const std::string& uri);
  virtual ~ccPocoWebsocket();

public:
  bool init(HTTPServerRequest& request, HTTPServerResponse& response);
  bool run(ccWebServerEventListener* server_event_listener);

public:
  virtual bool open(const std::string& uri);
  virtual bool close();
  virtual std::int32_t get_instance(); // It may be a Socket ID.
  virtual void* get_connection_info();
  virtual const std::string get_peer_ip();

  virtual bool send(const std::string& message);
  virtual bool send_binary(const void* buffer, std::size_t size);

private:
  //  ccWebServerEventListener* server_event_listener_;
  std::shared_ptr<Poco::Net::WebSocket> web_socket_;
};

class ccPocoWebsocketManager : public Luna::ccSingleton<ccPocoWebsocketManager>
{
public:
  ccPocoWebsocketManager() {}

public:
  void add_web_socket(std::shared_ptr<ccPocoWebsocket> web_socket) {
    mtx_.lock();
    web_sockets_.push_back(web_socket);
    mtx_.unlock();
  }

  void remove_web_socket(std::shared_ptr<ccPocoWebsocket> web_socket) {
    mtx_.lock();
  
    web_sockets_.erase(std::remove(web_sockets_.begin(), web_sockets_.end(), web_socket));

    mtx_.unlock();
  }

  void remove_web_socket(ccPocoWebsocket* web_socket) {
    mtx_.lock();

    for (auto it = web_sockets_.begin(); it != web_sockets_.end(); ++it) {
      if ((*it).get() == web_socket) {
        web_sockets_.erase(it);
        break;
      }
    }

    mtx_.unlock();
  }

private:
  std::vector<std::shared_ptr<ccPocoWebsocket>> web_sockets_;
  std::mutex mtx_;
};
}
