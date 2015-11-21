
#include "boost/bind.hpp"
#include "ccBoostWebServer.h"
#include "ccBoostWebServerSession.h"

#include "ccBoostWebServerRequest.h"
#include "ccBoostWebServerResponse.h"
#include "ccBoostWebServerHandler.h"

ccBoostWebServer::ccBoostWebServer(const char* name, const char* ports) :
        ccWebServer(name, ports), m_acceptor(m_io_service)
{
}

ccBoostWebServer::~ccBoostWebServer()
{
    Stop();
}

bool ccBoostWebServer::Stop()
{
	if (m_acceptor.is_open()) {
		m_acceptor.close();
	}
	m_work.reset();
	for each (auto th in m_thread_group)
	{
		th->join();
	}

	return true;
}

bool ccBoostWebServer::Start()
{
	DWORD dwResult = NO_ERROR;
	m_work.reset(new boost::asio::io_service::work(m_io_service));

	// THREAD_COUNT 개 까지 동시접속 가능
	for (size_t i = 0; i < THREAD_COUNT; ++i)
	{
		m_thread_group.push_back(new std::thread(boost::bind(&boost::asio::io_service::run, &m_io_service)));
	}
	dwResult = Listen();
	if (dwResult == NO_ERROR) {
		Accept();
	}

	return dwResult;
}

DWORD ccBoostWebServer::Listen()
{
	boost::asio::ip::tcp::resolver resolver(m_io_service);
	boost::asio::ip::tcp::resolver::query query("0.0.0.0", _strHttpPorts);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query, m_ec);
	if (m_ec) {
		return m_ec.value();
	}

	m_acceptor.open(endpoint.protocol(), m_ec);
	if (m_ec) {
		return m_ec.value();
	}

	m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	m_acceptor.bind(endpoint, m_ec);
	if (m_ec) {
		return m_ec.value();
	}

	m_acceptor.listen(boost::asio::socket_base::max_connections, m_ec);
	if (m_ec) {
		return m_ec.value();
	}

	return NO_ERROR;
}

VOID ccBoostWebServer::Accept()
{
	ccBoostWebServerSession::SP new_session = ccBoostWebServerSession::CreateSP(m_io_service, this->_pEventListener);

	m_acceptor.async_accept(
		new_session->GetSocket(),
		boost::bind(&ccBoostWebServer::AcceptComplete, this, new_session, boost::asio::placeholders::error));
}

VOID ccBoostWebServer::AcceptComplete(ccBoostWebServerSession::SP new_session, CONST boost::system::error_code& e)
{
	if (!e)
	{
		// 세션 시작
		new_session->HttpSessionStart();

		//다른 클라이언트의 접속을 기다린다.
		Accept();
	}
}
