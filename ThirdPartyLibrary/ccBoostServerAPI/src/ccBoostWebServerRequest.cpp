
#include "ccBoostWebServerRequest.h"
#include <regex>

ccBoostWebServerRequest::ccBoostWebServerRequest(const string& requestHeader, const string& requestBody)
	:_requestHeader(requestHeader), _requestBody(requestBody)
{
}

ccBoostWebServerRequest::~ccBoostWebServerRequest()
{
}


ccBoostWebServerRequest::HttpMethod  ccBoostWebServerRequest::GetMethod() const
{
	string method = GetMethod_c();
	if (strcmp("GET", method.c_str()) == 0)return HttpMethod_Get;
	if (strcmp("HEAD", method.c_str()) == 0)return HttpMethod_Head;
	if (strcmp("POST", method.c_str()) == 0)return HttpMethod_Post;
	if (strcmp("PUT", method.c_str()) == 0)return HttpMethod_Put;
	if (strcmp("DELETE", method.c_str()) == 0)return HttpMethod_Delete;
	if (strcmp("TRACE", method.c_str()) == 0)return HttpMethod_Trace;
	if (strcmp("CONNECT", method.c_str()) == 0)return HttpMethod_Connect;
	return HttpMethod_Unknown;
}

string ccBoostWebServerRequest::GetMethod_c() const
{
	std::regex rgxRequestLine("^(GET|HEAD|POST|PUT|DELETE|TRACE|CONNECT) (.+) HTTP/(\\d).(\\d)$");
	std::smatch matchRequestLine;
	if (!std::regex_search(_requestHeader, matchRequestLine, rgxRequestLine)) {
		return "";
	}
	return matchRequestLine[1];
}

string ccBoostWebServerRequest::GetURI() const
{
	std::regex rgxRequestLine("^(GET|HEAD|POST|PUT|DELETE|TRACE|CONNECT) (.+) HTTP/(\\d).(\\d)$");
	std::smatch matchRequestLine;
	if (!std::regex_search(_requestHeader, matchRequestLine, rgxRequestLine)) {
		return "";
	}
	return matchRequestLine[2];
}

string ccBoostWebServerRequest::GetQueryString() const
{
	throw new exception("Not Implemented");
}

string ccBoostWebServerRequest::GetPath() const
{
	throw new exception("Not Implemented");
}

string ccBoostWebServerRequest::GetResource() const
{
	throw new exception("Not Implemented");
}

bool ccBoostWebServerRequest::HasVar(const string& name) const
{
	throw new exception("Not Implemented");
}

string ccBoostWebServerRequest::GetVar(const string& name) const
{
	throw new exception("Not Implemented");
}

string ccBoostWebServerRequest::GetHeader(const string& name) const
{
	throw new exception("Not Implemented");
}

string ccBoostWebServerRequest::GetContentType() const
{
	throw new exception("Not Implemented");
}

unsigned long ccBoostWebServerRequest::GetContentLength() const
{
	return _requestBody.size();
}

// Retrieve info for the server interface that was connected with the remote device.
long ccBoostWebServerRequest::GetServerIp() const
{
	throw new exception("Not Implemented");
}

unsigned short  ccBoostWebServerRequest::GetServerPort() const
{
	throw new exception("Not Implemented");
}

long ccBoostWebServerRequest::GetRemoteIp() const
{
	throw new exception("Not Implemented");
}

const char* ccBoostWebServerRequest::GetPostData(unsigned long* size) const
{
	throw new exception("Not Implemented");
}