#pragma once

#include <memory>

#include "ccCore/ccSingleton.h"

#include "nlohmann/json.hpp"

class ccGCPIoTDevice {
public:
  ccGCPIoTDevice(const std::string &strSpecFile = "MyDeviceInfo.json");
  virtual ~ccGCPIoTDevice();

public:
  virtual bool start();
  virtual bool stop();

  virtual bool send(const nlohmann::json &protocol);

  virtual bool is_registered() {
    return is_connected_;
  }

protected:
  virtual bool set_device_command(nlohmann::json &protocol);
  virtual bool get_device_status_command(nlohmann::json &protocol);

protected:
  void retry_connect();

protected:  
  bool is_connected_;
  bool is_stop_by_user_;

  std::string target_uri_;
  std::map<std::string, std::function<bool(nlohmann::json &protocol)>> command_map_;
};
