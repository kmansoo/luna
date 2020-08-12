#include "ccPocoWebsocket.h"

using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;

#include "ccCore/ccSingleton.h"

namespace Luna {

ccPocoWebsocket::ccPocoWebsocket(const std::string& uri) : ccWebsocket(uri)
{
}

bool ccPocoWebsocket::init(HTTPServerRequest& request, HTTPServerResponse& response)
{
  try {
    web_socket_ = std::make_shared<Poco::Net::WebSocket>(request, response);
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

    return false;
  }


  return true;
}

bool ccPocoWebsocket::run(ccWebServerEventListener* server_event_listener)
{
  if (web_socket_ == nullptr) {
    return false;
  }

  std::thread thread = std::thread([](ccWebServerEventListener* server_event_listener, ccPocoWebsocket* ws, std::shared_ptr<Poco::Net::WebSocket> web_socket){
    bool is_ws_thread_stoped = false;

    Poco::Buffer<char> buffer(1028 * 8);
    int flags = 0;
    int read_size = 0;

    Poco::Timespan timeout(0, 500 * 1000);
    web_socket->setReceiveTimeout(timeout);
    
    while (is_ws_thread_stoped != true)
    {
      if (web_socket->poll(timeout, Poco::Net::Socket::SELECT_READ)) {
        try
        {
          read_size = web_socket->receiveFrame(buffer.begin(), static_cast<int>(buffer.size()), flags);
        }
        catch (Poco::Exception& exc)
        {
          std::cerr << __FUNCTION__ << ": " << exc.displayText() << std::endl;
          is_ws_thread_stoped = true;
          continue;
        }

        if (read_size <= 0 || (flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_CLOSE) {
          break;
        }

        if (read_size > 0) {
          server_event_listener->on_websocket_received_data(ws->get_instance(), buffer.begin(), read_size, flags == WebSocket::FRAME_TEXT);
        }
      }
    }

    is_ws_thread_stoped = true;

    if (server_event_listener != nullptr) {
      server_event_listener->on_websocket_disconnected(ws->get_instance());
    }

    ccPocoWebsocketManager::instance().remove_web_socket(ws);
    
  }, server_event_listener, this, web_socket_);

  thread.detach();

  return true;
}

ccPocoWebsocket::~ccPocoWebsocket()
{
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
