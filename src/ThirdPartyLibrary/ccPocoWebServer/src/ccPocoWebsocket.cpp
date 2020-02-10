#include "ccPocoWebsocket.h"

using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;

namespace Luna {

ccPocoWebsocket::ccPocoWebsocket(ccWebServerEventListener* server_event_listener, HTTPServerRequest& request, HTTPServerResponse& response)
    : ccWebsocket(request.getURI()), server_event_listener_(server_event_listener)
{
  try {
    web_socket_ = std::make_unique<Poco::Net::WebSocket>(request, response);

    is_ws_thread_stoped_ = false;

    ws_thread_ = std::thread([&]() {
      is_ws_thread_terminated_ = false;

      Poco::Buffer<char> buffer(4096);
      int flags = 0;
      int read_size = 0;;
      
      while (is_ws_thread_stoped_ != true)
      {
        try {
          std::cout << "call receiveFrame" << std::endl;

          read_size = web_socket_->receiveFrame(buffer.begin(), static_cast<int>(buffer.size()), flags);
        }
        catch (WebSocketException& exc)
        {
          std::cerr << "ccPocoWebsocket::ccPocoWebsocket, thread: " << exc.displayText() << std::endl;
          is_ws_thread_stoped_ = true;
          continue;
        }

        if (read_size <= 0 || (flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_CLOSE) {
          break;
        }

        // if (read_size > 0) {
        //   server_event_listener_->on_websocket_received_data(get_instance(), std::string(buffer.begin(), read_size));
        // }

        std::cout << "get next data.." << std::endl;
      }

      is_ws_thread_terminated_ = true;

      if (server_event_listener_ != nullptr) {
        server_event_listener_->on_websocket_disconnected(get_instance());
      }

      std::cout << "End of thread" << std::endl;
    });
  }
  catch (WebSocketException& exc)
  {
    switch (exc.code()) {
    case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
      response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
      // fallthrough

    case WebSocket::WS_ERR_NO_HANDSHAKE:
    case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
    case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
      response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
      response.setContentLength(0);
      response.send();
      break;
    }
  }
}

ccPocoWebsocket::~ccPocoWebsocket()
{
  is_ws_thread_stoped_ = true;

  if (web_socket_) {
    web_socket_->close();
  }
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
{
  if (web_socket_ == nullptr) {
    return -1;
  }

  return web_socket_->impl()->sockfd();
}

const std::string ccPocoWebsocket::get_peer_ip()
{
  if (web_socket_ == nullptr) {
    return "";
  }

  return web_socket_->peerAddress().toString();
}

void* ccPocoWebsocket::get_connection_info()
{
  if (web_socket_ == nullptr) {
    return nullptr;
  }

  return web_socket_.get();
}

bool ccPocoWebsocket::send(const std::string& message)
{
  if (web_socket_ == nullptr) {
    return false;
  }

  try {
    web_socket_->sendFrame(message.c_str(), message.length());
  }
  catch (WebSocketException& exc)
  {
    std::cerr << __FUNCTION__ << ": " << exc.displayText() << std::endl;
    return false;
  }

  return true;
}

bool ccPocoWebsocket::send_binary(const void* buffer, std::size_t size)
{
  if (web_socket_ == nullptr) {
    return false;
  }

  try {
    web_socket_->sendBytes(buffer, size);
  }
  catch (WebSocketException& exc)
  {
    std::cerr << __FUNCTION__ << ": " << exc.displayText() << std::endl;
    return false;
  }

  return true;
}
}
