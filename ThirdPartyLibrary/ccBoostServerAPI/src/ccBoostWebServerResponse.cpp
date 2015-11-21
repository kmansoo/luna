
#include "ccBoostWebServerResponse.h"

ccBoostWebServerResponse::ccBoostWebServerResponse()
{
}

ccBoostWebServerResponse::~ccBoostWebServerResponse()
{
}

void ccBoostWebServerResponse::SetResponseString(std::string & data)
{
	_data.assign(data);
}

std::string& ccBoostWebServerResponse::GetResponseString()
{
	return _data;
}