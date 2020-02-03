#pragma once

#include "ccWebServer/ccWebsocket.h"

namespace Luna {

class ccPocoWebsocket : public ccWebsocket {
public:
  ccPocoWebsocket(const std::string& uri);
  ccPocoWebsocket(const char* uri, std::size_t size);

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
  int socket_id_;
};
}
