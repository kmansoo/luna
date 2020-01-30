/*
 * ccMongooseWebServerRequest.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#ifndef THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERREQUEST_H_
#define THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERREQUEST_H_

#include "ccWebServer/ccWebServerRequest.h"

#include "mongoose/mongoose.h"

#include <vector>

namespace Luna {

class ccMongooseWebServerRequest : public ccWebServerRequest {
public:
  ccMongooseWebServerRequest(struct mg_connection* con, struct http_message* http_msg);
  virtual ~ccMongooseWebServerRequest();

public:
  virtual std::string get_query_string();
  virtual std::string get_path();
  virtual std::string get_resource();

  virtual bool has_variable(const std::string& name) const;
  virtual std::string get_variable(const std::string& name);

  virtual std::string get_header(const std::string& name);

  virtual std::string get_content_type();
  virtual unsigned long get_content_length();

  virtual long get_content_body(std::string& strBody);
  virtual long get_content_body(char* pBuf, std::size_t size);

  // Retrieve info for the server interface that was connected with the remote device.
  virtual long get_server_ip() const;
  virtual unsigned short get_server_port() const;
  virtual long get_remote_ip() const;

private:
  void splite_path_pos() const;

  bool has_variable_connection(const std::string& name) const;
  std::string get_variable_connection(const std::string& name);

private:
  struct mg_connection* mg_connection_;
  struct http_message* mg_http_message_;

  mutable char const* path_pos_;
  mutable char const* path_end_;

  mutable std::vector<char> post_buffer_;
};
}

#endif /* THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERREQUEST_H_ */
