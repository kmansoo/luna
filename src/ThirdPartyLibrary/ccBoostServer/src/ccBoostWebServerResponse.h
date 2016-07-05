
#ifndef THIRDPARTYLIBRARY_CCBOOSTSERVERAPI_CCBOOSTWEBSERVERRESPONSE_H_
#define THIRDPARTYLIBRARY_CCBOOSTSERVERAPI_CCBOOSTWEBSERVERRESPONSE_H_

#include "ccWebServerAPI/ccWebServerResponse.h"

class ccBoostWebServerResponse : public ccWebServerResponse
{
public:
	ccBoostWebServerResponse();
	~ccBoostWebServerResponse();

	void SetResponseString(std::string& data);
	std::string& GetResponseString();
private:
	string _data;
};

#endif /* THIRDPARTYLIBRARY_CCBOOSTSERVERAPI_CCBOOSTWEBSERVERRESPONSE_H_ */