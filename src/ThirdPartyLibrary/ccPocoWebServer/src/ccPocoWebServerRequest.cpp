#include <sstream>

#include "ccPocoWebServerRequest.h"

#include "Poco/URI.h"
#include "Poco/StreamCopier.h"

using Poco::URI;
using Poco::StreamCopier;

namespace Luna {

ccPocoWebServerRequest::ccPocoWebServerRequest(const HTTPServerRequest& request)
    : server_request_(request)
{
  method_ = server_request_.getMethod();
  uri_ = server_request_.getURI();

  parsed_uri_ = URI(uri_);
}

ccPocoWebServerRequest::~ccPocoWebServerRequest()
{
  // TODO Auto-generated destructor stub
}

std::string ccPocoWebServerRequest::get_query_string()
{
  return parsed_uri_.getRawQuery();
}

std::string ccPocoWebServerRequest::get_path()
{
  return parsed_uri_.getPath();
}

std::string ccPocoWebServerRequest::get_resource()
{
  std::vector <std::string> segments;

  parsed_uri_.getPathSegments(segments);

  if (segments.size() == 0) {
    return blank_string_data_;
  }

  return segments[segments.size() - 1];
}

bool ccPocoWebServerRequest::has_variable(const std::string& name) const
{
  URI::QueryParameters params = parsed_uri_.getQueryParameters();

  for (auto& param : params) {
    if (param.first == name)
      return true;
  }

  return false;
}

std::string ccPocoWebServerRequest::get_variable(const std::string& name)
{
  URI::QueryParameters params = parsed_uri_.getQueryParameters();

  for (auto& param : params) {
    if (param.first == name)
      return param.second;
  }

  return blank_string_data_;
}

std::string ccPocoWebServerRequest::get_header(const std::string& name)
{
  auto item = server_request_.find(name);

	if (item == server_request_.end()) {
    return blank_string_data_;
  }

  return item->second;
}

std::string ccPocoWebServerRequest::get_content_type()
{
  return server_request_.getContentType();
}

unsigned long ccPocoWebServerRequest::get_content_length()
{
  return server_request_.getContentLength();
}

// Retrieve info for the server interface that was connected with the remote device.
long ccPocoWebServerRequest::get_server_ip() const
{
  return 0;
}

unsigned short ccPocoWebServerRequest::get_server_port() const
{
  return 0;
}

long ccPocoWebServerRequest::get_remote_ip() const
{
  return 0;
}

long ccPocoWebServerRequest::get_content_body(std::string& body)
{
  StreamCopier::copyToString(((HTTPServerRequest*)&server_request_)->stream(), body);

  return body.length();
}

long ccPocoWebServerRequest::get_content_body(char* body_buffer, std::size_t size)
{
  std::string body;

  get_content_body(body);

  std::size_t copy_size = (size > body.length()) ? body.length() : size;

  memcpy(body_buffer, body.c_str(), copy_size);

  return copy_size;
}

}

