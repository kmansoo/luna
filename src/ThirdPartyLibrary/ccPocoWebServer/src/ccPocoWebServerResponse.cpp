#include "ccPocoWebServerResponse.h"

namespace Luna {

ccPocoWebServerResponse::ccPocoWebServerResponse(HTTPServerResponse& response) : server_response_(response)
{
}

ccPocoWebServerResponse::~ccPocoWebServerResponse()
{
  close_without_content();
}

void ccPocoWebServerResponse::check_header_sent()
{
  is_haders_sent_ = true;
}

void ccPocoWebServerResponse::send_status(unsigned int code, const std::string& status_text, bool has_content)
{
  server_response_.setStatus((HTTPResponse::HTTPStatus)code);
  server_response_.setReason(status_text);

  if (has_content == false)
    server_response_.setContentLength(0);
}

void ccPocoWebServerResponse::send_status(unsigned int code, const std::string& status_text, const std::string& ext_info)
{
  server_response_.setStatus((HTTPResponse::HTTPStatus)code);
  server_response_.setReason(status_text);

  server_response_.setContentLength(ext_info.length());

  this->write_content_to_connector(ext_info.c_str(), ext_info.length());
}

void ccPocoWebServerResponse::send_content_type(const std::string& type, bool has_separator)
{
  server_response_.setContentType(type);
}

void ccPocoWebServerResponse::send_content_type(const std::string& type, size_t size, bool has_separator)
{
  server_response_.setContentLength(size);
  server_response_.setContentType(type);
}

size_t ccPocoWebServerResponse::send_file(const std::string& path, const std::string& media_type)
{
  server_response_.sendFile(path, media_type);

  return 1;
}

size_t ccPocoWebServerResponse::write_content_to_connector(const char* body_buffer, size_t size)
{
  if (server_response_.sent() == false && ostr_ == nullptr) {
    ostr_ = &server_response_.send();
  }

  if (size == 0) {
    return 0;
  }

  ostr_->write(body_buffer, size);
  
  return size;
}

void ccPocoWebServerResponse::close_without_content()
{
  if (server_response_.sent() == false) {
    server_response_.send();
  }
}

}
