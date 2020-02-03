/*
 * ccMongooseWebServerRequest.cpp
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#include "ccMongooseWebServerRequest.h"

namespace Luna {

ccMongooseWebServerRequest::ccMongooseWebServerRequest(struct mg_connection* con, struct http_message* http_msg)
    : mg_connection_(con)
    , mg_http_message_(http_msg)
    , path_pos_(NULL)
    , path_end_(NULL)
{
  if (mg_http_message_ != NULL) {
    method_.assign(mg_http_message_->method.p, mg_http_message_->method.len);
    uri_.assign(mg_http_message_->uri.p, mg_http_message_->uri.len);
  }
}

ccMongooseWebServerRequest::~ccMongooseWebServerRequest()
{
  // TODO Auto-generated destructor stub
}

std::string ccMongooseWebServerRequest::get_query_string()
{
  if (mg_http_message_ == NULL)
    return blank_string_data_;

  std::string strQuery(mg_http_message_->query_string.p, mg_http_message_->query_string.len);

  return strQuery;
}

std::string ccMongooseWebServerRequest::get_path()
{
  if (mg_http_message_ == NULL)
    return blank_string_data_;

  splite_path_pos();

  std::string strPath = uri_;

  if (path_pos_ != NULL)
    strPath += path_pos_;

  return strPath;
}

std::string ccMongooseWebServerRequest::get_resource()
{
  if (mg_connection_ == NULL)
    return blank_string_data_;

  splite_path_pos();

  return std::string(path_pos_, path_end_);
}

bool ccMongooseWebServerRequest::has_variable(const std::string& name) const
{
  return has_variable_connection(name);
}

std::string ccMongooseWebServerRequest::get_variable(const std::string& name)
{
  return get_variable_connection(name);
}

std::string ccMongooseWebServerRequest::get_header(const std::string& name)
{
  if (mg_http_message_ == NULL)
    return blank_string_data_;

  struct mg_str* pResult = mg_get_http_header(mg_http_message_, name.c_str());

  if (pResult == NULL)
    return blank_string_data_;

  return std::string(pResult->p, pResult->len);
}

std::string ccMongooseWebServerRequest::get_content_type()
{
  return get_header("Content-Type");
}

unsigned long ccMongooseWebServerRequest::get_content_length()
{
  std::string strContentLength = get_header("Content-Length");

  return atol(strContentLength.c_str());
}

// Retrieve info for the server interface that was connected with the remote device.
long ccMongooseWebServerRequest::get_server_ip() const
{
  return 0;
}

unsigned short ccMongooseWebServerRequest::get_server_port() const
{
  return 0;
}

long ccMongooseWebServerRequest::get_remote_ip() const
{
  return 0;
}

long ccMongooseWebServerRequest::get_content_body(std::string& strBody)
{
  if (mg_http_message_ == NULL)
    return 0;

  strBody.append(mg_http_message_->body.p, mg_http_message_->body.len);

  return mg_http_message_->body.len;
}

long ccMongooseWebServerRequest::get_content_body(char* pBuf, std::size_t size)
{
  if (mg_http_message_ == NULL || pBuf == NULL)
    return 0;

  std::size_t copy_size = (size > mg_http_message_->body.len) ? mg_http_message_->body.len : size;

  memcpy(pBuf, mg_http_message_->body.p, copy_size);

  return copy_size;
}

//  private method
void ccMongooseWebServerRequest::splite_path_pos() const
{
  if (path_pos_ || mg_http_message_ == NULL)
    return;

  char const* slash = 0;
  char const* it = mg_http_message_->uri.p;

  for (std::size_t nIndex = 0; nIndex < mg_http_message_->uri.len; nIndex++) {
    if (*it == '/')
      slash = it;

    ++it;
  }

  path_end_ = it;
  path_pos_ = slash == 0 ? path_end_ : slash + 1;
}

bool ccMongooseWebServerRequest::has_variable_connection(const std::string& name) const
{
  if (mg_http_message_ == NULL)
    return false;

  char outbuf[CV_MAXGETSIZE];

  return mg_get_http_var(&mg_http_message_->query_string, name.c_str(), &outbuf[0], CV_MAXGETSIZE) > 0;
}

std::string ccMongooseWebServerRequest::get_variable_connection(const std::string& name)
{
  std::string variable_data;

  variable_data.resize(CV_MAXGETSIZE);

  int sz = -1;

  if (mg_http_message_ != NULL)
    sz = mg_get_http_var(&mg_http_message_->query_string, name.c_str(), &variable_data[0], variable_data.size());

  if (sz == -1)
    return blank_string_data_;

  variable_data.resize(sz);

  return variable_data;
}
}
