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

protected:
  virtual size_t write_content_to_connector(const char* body_buffer, size_t size);

private:
  HTTPServerResponse& server_response_;
};
}
