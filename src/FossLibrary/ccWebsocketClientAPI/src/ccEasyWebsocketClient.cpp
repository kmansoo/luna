#include <iostream>

#include "ccEasyWebsocketClient.h"

ccEasyWebsocketClient::ccEasyWebsocketClient()
{
    _bIsStopThread = false;
}

ccEasyWebsocketClient::~ccEasyWebsocketClient()
{
    Close();
}

bool ccEasyWebsocketClient::Open(const std::string& strUri)
{
    if (_pWorkWS != NULL)
        return false;

    _pWorkWS.reset(easywsclient::WebSocket::from_url(strUri));
    
    if (_pWorkWS == NULL)
        return false;
    
    _bIsStopThread = false;
    _oPollThread = std::thread(std::bind(&ccEasyWebsocketClient::DoPoll, this));
    
    return true;
}

bool ccEasyWebsocketClient::Close()
{
    if (_bIsStopThread == true)
        return false;

    if (_pWorkWS == NULL)
        return false;

    _bIsStopThread = true;
    _oPollThread.join();

    _pWorkWS->close();
    _pWorkWS.reset();

    return true;
}

std::int32_t ccEasyWebsocketClient::GetInstance()  // It may be a Socket ID. 
{
    if (_pWorkWS == NULL)
        return -1;

    return _pWorkWS->getInstance();
}


bool ccEasyWebsocketClient::Send(const std::string& strMessage)
{
    if (_pWorkWS == NULL)
        return false;

    _pWorkWS->send(strMessage);

    return true;
}

bool ccEasyWebsocketClient::SendBinary(const void* pBuffer, std::size_t size)
{
    if (_pWorkWS == NULL)
        return false;

    std::vector<uint8_t> aSendData;

    for (std::size_t nIndex = 0; nIndex < size; nIndex++)
        aSendData.push_back(((uint8_t*)pBuffer)[nIndex]);

    _pWorkWS->sendBinary(aSendData);

    return true;
}

void ccEasyWebsocketClient::DoReceive(const std::string& message)
{
    if (!_oEventListener)
        std::cout << "RECV: " << message << std::endl;
    else
        _oEventListener(ccWebSocketEvent_ReceivedData, message);
}

void ccEasyWebsocketClient::DoPoll()
{
    easywsclient::WebSocket::readyStateValues eConnectStatus = _pWorkWS->getReadyState();

    while (_bIsStopThread == false)
    {
        if (_bIsStopThread == true)
            std::this_thread::sleep_for(std::chrono::microseconds{ 10 });

        if (_pWorkWS == NULL)
            break;

        if (_pWorkWS->getReadyState() != eConnectStatus)
        {
            switch (_pWorkWS->getReadyState())
            {
            case easywsclient::WebSocket::OPEN:
                if (!_oEventListener)
                    std::cout << "easywsclient::WebSocket::OPEN " << std::endl;
                else
                    _oEventListener(ccWebSocketEvent_Connected, _oNullData);
                break;

            case easywsclient::WebSocket::CLOSED:
                _bIsStopThread = false;

                if (!_oEventListener)
                    std::cout << "easywsclient::WebSocket::CLOSED " << std::endl;
                else
                    _oEventListener(ccWebSocketEvent_Disconnected, _oNullData);
                break;

            default:
                break;
            }

            eConnectStatus = _pWorkWS->getReadyState();
        }

        if (_pWorkWS->getReadyState() != easywsclient::WebSocket::CLOSED)
        {
            _pWorkWS->poll(100);
            _pWorkWS->dispatch(std::bind(&ccEasyWebsocketClient::DoReceive, this, std::placeholders::_1));
        }
    }
}
