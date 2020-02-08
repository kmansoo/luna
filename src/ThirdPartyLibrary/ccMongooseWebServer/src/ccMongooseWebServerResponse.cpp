/*
 * ccMongooseWebServerResponse.cpp
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#include <string>

#include "ccCore/ccFileStream.h"
#include "ccCore/ccFileInfoSplitter.h"

#include "ccMongooseWebServerResponse.h"

namespace Luna {

ccMongooseWebServerResponse::ccMongooseWebServerResponse(struct mg_connection* con)
    : mg_connection_(con)
{
}

ccMongooseWebServerResponse::~ccMongooseWebServerResponse()
{
  // TODO Auto-generated destructor stub
}

size_t ccMongooseWebServerResponse::send_file(const std::string& path, const std::string& media_type)
{
  Luna::ccFileInfoSplitter info_splitter(path);
  Luna::ccFileStream f; 

	std::string attached_file_info = "Disposition: attachment; filename=\"";
	attached_file_info += info_splitter.file_name_;
	attached_file_info += "\"";
  attached_file_info += "\r\n";

	write_content_to_connector(attached_file_info.c_str(), attached_file_info.length());

  if (f.open(path, "rb") == false) {
    this->send_content_type(media_type, 0, false);
    return 0;
  }

  uint32_t file_size = f.length();

  this->send_content_type(media_type, file_size, true);

  char buffer[4096];

  while (f.eof() == false) {
    uint32_t read_size = f.read(buffer, 4096);

    write_content_to_connector(buffer, read_size);
  }

  return file_size;
}

size_t ccMongooseWebServerResponse::write_content_to_connector(const char* strBuf, size_t size)
{
  mg_send(mg_connection_, strBuf, size);

  return size;
}
}
