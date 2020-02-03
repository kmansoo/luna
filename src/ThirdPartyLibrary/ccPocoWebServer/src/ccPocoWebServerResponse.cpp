#include "ccPocoWebServerResponse.h"

namespace Luna {

ccPocoWebServerResponse::ccPocoWebServerResponse(HTTPServerResponse& response) : server_response_(response)
{
}

ccPocoWebServerResponse::~ccPocoWebServerResponse()
{

}

size_t ccPocoWebServerResponse::write_content_to_connector(const char* body_buffer, size_t size)
{
  server_response_.sendBuffer(body_buffer, size);
  
  return size;
}
}
