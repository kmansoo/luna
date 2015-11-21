
#ifndef THIRDPARTYLIBRARY_CCBOOSTSERVERAPI_CCBOOSTWEBSERVER_H_
#define THIRDPARTYLIBRARY_CCBOOSTSERVERAPI_CCBOOSTWEBSERVER_H_

#include <thread>
#include "boost/asio.hpp"
#include "ccWebServerAPI/ccWebServer.h"
#include "ccBoostWebServerHandler.h"
#include "boost/enable_shared_from_this.hpp"
#include "ccBoostWebServerSession.h"

class ccBoostWebServer : public ccWebServer
{
public:
    ccBoostWebServer(const char* name, const char* ports);
    virtual ~ccBoostWebServer();

	bool Start() override;
	bool Stop() override;
	DWORD Listen();
	VOID Accept();
	VOID AcceptComplete(ccBoostWebServerSession::SP new_session, CONST boost::system::error_code& e);

private:	
	boost::system::error_code						m_ec;
	boost::asio::io_service							m_io_service;
	std::shared_ptr<boost::asio::io_service::work>	m_work;
	boost::asio::ip::tcp::acceptor					m_acceptor;
	std::vector<std::thread*>						m_thread_group;

private:
	CONST int THREAD_COUNT = 3;
};

#endif /* THIRDPARTYLIBRARY_CCBOOSTSERVERAPI_CCBOOSTWEBSERVER_H_ */
