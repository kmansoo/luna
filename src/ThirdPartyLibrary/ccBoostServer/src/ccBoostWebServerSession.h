#pragma once

#include "stdafx.h"
#include "boost\asio.hpp"
#include "boost\enable_shared_from_this.hpp"
#include "ccBoostWebServerRequest.h"
#include "ccBoostWebServerResponse.h"
#include "ccBoostWebServerHandler.h"
#include "ccWebServerAPI\ccWebServerEventListener.h"

class ccBoostWebServerSession : public boost::enable_shared_from_this<ccBoostWebServerSession>
{
public:
	typedef boost::shared_ptr<ccBoostWebServerSession> SP;
	static SP CreateSP(boost::asio::io_service& io_service, ccWebServerEventListener* eventListener)
	{
		return SP(new ccBoostWebServerSession(io_service, eventListener));
	}
private:
	ccBoostWebServerSession(boost::asio::io_service& io_service, ccWebServerEventListener* eventListener)
		: m_socket(io_service), m_eventListener(eventListener)
	{
	}

public:
	VOID HttpSessionStart();
	VOID Disconnect();

private:
	VOID ReadHeader();
	VOID ReadHeaderComplete(CONST boost::system::error_code& e, std::size_t nBytesTransferred);

	VOID ReadBody(size_t contentSize);
	VOID ReadBodyComplete(CONST boost::system::error_code& e, std::size_t nBytesTransferred);

	VOID Handle();

	VOID Write(ccBoostWebServerResponse& res);
	VOID WriteComplete(CONST boost::system::error_code& e, std::size_t nBytesTransferred);

public:
	boost::asio::ip::tcp::socket& GetSocket() {
		return m_socket;
	}

private:
	boost::asio::ip::tcp::socket				m_socket;
	boost::asio::streambuf						m_request_buffer;
	std::vector<char>							m_response_buffer;
	std::string									m_requestHeader;
	std::string									m_requestBody;
	ccWebServerEventListener*					m_eventListener;
};
