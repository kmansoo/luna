#pragma once

#include "ccWebServer/ccWebServerResponse.h"

#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPResponse.h"

using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServerResponse;

namespace Luna {

class ccPocoWebServerResponse : public ccWebServerResponse {
public:
  ccPocoWebServerResponse(HTTPServerResponse& response);
  virtual ~ccPocoWebServerResponse();

public:
  virtual void send_status(unsigned int code, const std::string& status_text, bool has_content = false);
  virtual void send_status(unsigned int code, const std::string& status_text, const std::string& ext_info);

  virtual void send_content_type(const std::string& type, bool has_separator = true);
  virtual void send_content_type(const std::string& type, size_t size, bool has_separator = true);
  virtual void close_without_content();
  
protected:
  virtual size_t write_content_to_connector(const char* body_buffer, size_t size);
   virtual void  check_header_sent();

private:
  HTTPServerResponse& server_response_;
  std::ostream* ostr_ = nullptr;
};
}
