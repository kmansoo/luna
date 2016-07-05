#pragma once
#include "src\ccBoostWebServerResponse.h"
#include "src\ccBoostWebServerRequest.h"

class ccBoostWebServerHandler
{
public:
	virtual int Handle(const ccBoostWebServerRequest& req, ccBoostWebServerResponse& res) = 0;
};