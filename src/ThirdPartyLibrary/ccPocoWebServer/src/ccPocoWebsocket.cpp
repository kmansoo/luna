#include "ccPocoWebsocket.h"

namespace Luna {

ccPocoWebsocket::ccPocoWebsocket(const std::string& uri)
    : ccWebsocket(uri)
{
  // if (mg_connection_)
  //   socket_id_ = mg_connection_->sock;
}

ccPocoWebsocket::ccPocoWebsocket(const char* uri, std::size_t size)
    : ccWebsocket(std::string(uri, size))
{

  // if (mg_connection_)
  //   socket_id_ = mg_connection_->sock;
}

ccPocoWebsocket::~ccPocoWebsocket()
{
}

bool ccPocoWebsocket::open(const std::string& uri)
{
  return false;
}

bool ccPocoWebsocket::close()
{
  return false;
}

std::int32_t ccPocoWebsocket::get_instance()
{ // It may be a Socket ID.
  // if (mg_connection_ == NULL)
  //   return -1;

  return socket_id_;
}

const std::string ccPocoWebsocket::get_peer_ip()
{
  char addr[32];

  // mg_sock_addr_to_str(&mg_connection_->sa, addr, sizeof(addr), MG_SOCK_STRINGIFY_IP);

  return std::string(addr);
}

void* ccPocoWebsocket::get_connection_info()
{
  // return mg_connection_;
  return nullptr;
}

bool ccPocoWebsocket::send(const std::string& message)
{
  // if (mg_connection_ == NULL)
  //   return false;

  // mg_send_websocket_frame(mg_connection_, WEBSOCKET_OP_TEXT, message.c_str(), message.length());

  return true;
}

bool ccPocoWebsocket::send_binary(const void* buffer, std::size_t size)
{
  // if (mg_connection_ == NULL)
  //   return false;

  // mg_send_websocket_frame(mg_connection_, WEBSOCKET_OP_BINARY, buffer, size);

  return true;
}
}
