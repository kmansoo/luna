#include <iostream>

#include "ccWebsocketClient/ccEasyWebsocketClient.h"

namespace Luna {

ccEasyWebsocketClient::ccEasyWebsocketClient() {
    is_stop_thread_ = false;
}

ccEasyWebsocketClient::ccEasyWebsocketClient(const std::string& uri) {
    open(uri);
}


ccEasyWebsocketClient::~ccEasyWebsocketClient() {
    close();
}

bool ccEasyWebsocketClient::open(const std::string& uri) {
    if (websocket_ != NULL)
        return false;

    websocket_.reset(easywsclient::WebSocket::from_url(uri));

    if (websocket_ == NULL)
        return false;

    is_stop_thread_ = false;
    polling_thread_ = std::thread(std::bind(&ccEasyWebsocketClient::poll, this));

    return true;
}

bool ccEasyWebsocketClient::close() {
    if (is_stop_thread_ == true)
        return false;

    if (websocket_ == NULL)
        return false;

    is_stop_thread_ = true;
    polling_thread_.join();

    websocket_->close();
    websocket_.reset();

    return true;
}

std::int32_t ccEasyWebsocketClient::get_instance()  // It may be a Socket ID. 
{
    if (websocket_ == NULL)
        return -1;

    return websocket_->getInstance();
}

const std::string ccEasyWebsocketClient::get_peer_ip() {

    return std::string("");
}



bool ccEasyWebsocketClient::send(const std::string& message) {
    if (websocket_ == NULL)
        return false;

    websocket_->send(message);

    return true;
}

bool ccEasyWebsocketClient::send_binary(const void* buffer, std::size_t size) {
    if (websocket_ == NULL)
        return false;

    std::vector<uint8_t> send_data;

    for (std::size_t nIndex = 0; nIndex < size; nIndex++)
        send_data.push_back(((uint8_t*)buffer)[nIndex]);

    websocket_->send_binary(send_data);

    return true;
}

void ccEasyWebsocketClient::receive(const std::string& message) {
    if (!event_listener_)
        std::cout << "RECV: " << message << std::endl;
    else
        event_listener_(ccWebSocketEvent_ReceivedData, message.c_str(), message.size(), true);
}

void ccEasyWebsocketClient::poll() {
    easywsclient::WebSocket::readyStateValues connect_status = easywsclient::WebSocket::CLOSED;

    while (is_stop_thread_ == false) {
        if (is_stop_thread_ == true)
            std::this_thread::sleep_for(std::chrono::microseconds{ 10 });

        if (websocket_ == NULL)
            break;

        if (websocket_->getReadyState() != connect_status) {
            switch (websocket_->getReadyState()) {
            case easywsclient::WebSocket::OPEN:
                if (!event_listener_)
                    std::cout << "easywsclient::WebSocket::OPEN " << std::endl;
                else
                    event_listener_(ccWebSocketEvent_Connected, blank_string_data_.c_str(), 0, false);
                break;

            case easywsclient::WebSocket::CLOSED:
                is_stop_thread_ = false;

                if (!event_listener_)
                    std::cout << "easywsclient::WebSocket::CLOSED " << std::endl;
                else
                    event_listener_(ccWebSocketEvent_Disconnected, blank_string_data_.c_str(), 0, false);
                break;

            default:
                break;
            }

            connect_status = websocket_->getReadyState();
        }

        if (websocket_->getReadyState() != easywsclient::WebSocket::CLOSED) {
            websocket_->poll(100);
            websocket_->dispatch(std::bind(&ccEasyWebsocketClient::receive, this, std::placeholders::_1));
        }
    }
}

}
