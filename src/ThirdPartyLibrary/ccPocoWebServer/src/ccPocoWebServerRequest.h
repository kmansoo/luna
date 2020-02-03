#pragma once

#include "ccWebServer/ccWebServerRequest.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/URI.h"

using Poco::URI;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPServerRequest;

#include <vector>

namespace Luna {

class ccPocoWebServerRequest : public ccWebServerRequest {
public:
  ccPocoWebServerRequest(const HTTPServerRequest& request);
  virtual ~ccPocoWebServerRequest();

public:
  virtual std::string get_query_string();
  virtual std::string get_path();
  virtual std::string get_resource();

  virtual bool has_variable(const std::string& name) const;
  virtual std::string get_variable(const std::string& name);

  virtual std::string get_header(const std::string& name);

  virtual std::string get_content_type();
  virtual unsigned long get_content_length();

  virtual long get_content_body(std::string& body);
  virtual long get_content_body(char* body_buffer, std::size_t size);

  // Retrieve info for the server interface that was connected with the remote device.
  virtual long get_server_ip() const;
  virtual unsigned short get_server_port() const;
  virtual long get_remote_ip() const;

private:
  const HTTPServerRequest& server_request_;
  URI parsed_uri_;
 
  mutable std::vector<char> post_buffer_;
};
}
