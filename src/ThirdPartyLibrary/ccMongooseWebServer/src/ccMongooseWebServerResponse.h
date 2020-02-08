/*
 * ccMongooseWebServerResponse.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#ifndef THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERRESPONSE_H_
#define THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERRESPONSE_H_

#include "ccWebServer/ccWebServerResponse.h"

#include "mongoose/mongoose.h"

namespace Luna {

class ccMongooseWebServerResponse : public ccWebServerResponse {
public:
  ccMongooseWebServerResponse(struct mg_connection* connection);
  virtual ~ccMongooseWebServerResponse();

public:
  virtual size_t send_file(const std::string& path, const std::string& media_type);

protected:
  virtual size_t write_content_to_connector(const char* strBuf, size_t size);

private:
  struct mg_connection* mg_connection_;
};
}

#endif /* THIRDPARTYLIBRARY_CCMONGOOSESERVERAPI_CCMONGOOSEWEBSERVERRESPONSE_H_ */
