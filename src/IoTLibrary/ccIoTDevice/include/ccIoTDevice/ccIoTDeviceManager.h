/*
 * ccRESTfulChattingApi.h
 *
 *  Created on: 2015. 11. 28.
 *      Author: kmansoo
 */

#ifndef CCPRODUCTLIBRARY_CCIOTDEVICEMANAGERAPI_CCIOTDEVICEMANAGER_H_
#define CCPRODUCTLIBRARY_CCIOTDEVICEMANAGERAPI_CCIOTDEVICEMANAGER_H_

#include <string>
#include <functional>
#include <memory>
#include <map>
#include <mutex>

#include "ccWebServer/ccRESTfulApi.h"
#include "ccWebServer/ccWebsocketManager.h"

#include "ccIoTDeviceProtocol.h"

#include "ccIoTDeviceAgent.h"

class ccIoTDeviceManager {
public:
    ccIoTDeviceManager();
    virtual ~ccIoTDeviceManager();

public:
    void    show();
    void    control_all_switches(bool on);
    void    control_all_lights(bool on);
    void    control_all_locks(bool open);

protected:
    //  RESTful API Function
    bool    devices(std::shared_ptr<Luna::ccWebServerRequest> request, std::shared_ptr<Luna::ccWebServerResponse> response);
    bool    devices_device(std::shared_ptr<Luna::ccWebServerRequest> request, std::shared_ptr<Luna::ccWebServerResponse> response);

protected:
    //  Websocket Function
    bool    ws_iot_device(Luna::ccWebsocket::ccWebSocketEvent ws_event, std::shared_ptr<Luna::ccWebsocket> websocket, const std::string data);

protected:
    bool    find_device_agent(std::shared_ptr<Luna::ccWebsocket> websocket, std::shared_ptr<ccIoTDeviceAgent>& device_agent);
    bool    parse_command(std::shared_ptr<Luna::ccWebsocket> websocket, const std::string data);

protected:
    virtual bool    register_command(std::shared_ptr<Luna::ccWebsocket> websocket, ccIoTDeviceProtocol& protocol);
    virtual bool    deregister_command(std::shared_ptr<Luna::ccWebsocket> websocket, ccIoTDeviceProtocol& protocol);
    virtual bool    get_device_status_command(std::shared_ptr<Luna::ccWebsocket> websocket, ccIoTDeviceProtocol& protocol);
    virtual bool    update_device_status_command(std::shared_ptr<Luna::ccWebsocket> websocket, ccIoTDeviceProtocol& protocol);
    virtual bool    set_control_command(std::shared_ptr<Luna::ccWebsocket> websocket, ccIoTDeviceProtocol& protocol);

protected:
    std::map < std::int32_t, std::shared_ptr<ccIoTDeviceAgent> > agent_map_;

    std::mutex  mtx_;

    std::shared_ptr<Luna::ccRESTfulApi>       restful_api_;
    std::shared_ptr<Luna::ccWebsocketManager> websocket_manager_;

    std::map < std::string, std::function<bool(std::shared_ptr<Luna::ccWebsocket> websocket, ccIoTDeviceProtocol& protocol)>> command_map_;
};


#endif /* CCPRODUCTLIBRARY_CCIOTDEVICEMANAGERAPI_CCIOTDEVICEMANAGER_H_ */
