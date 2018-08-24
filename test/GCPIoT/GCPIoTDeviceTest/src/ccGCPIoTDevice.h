#pragma once

#include <memory>

#include "ccCore/ccSingleton.h"

#include "ccMqttClient/MQTTClient.h"

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

private:
  bool loadSpecFile(const std::string& file);

  bool initMqttClient();
  bool clearMqttClient();

  bool sendMqttTopic(const std::string& payload);

protected:  
  bool is_connected_;
  bool is_stop_by_user_;

  nlohmann::json              device_spec_;
  MQTTClient                  mqtt_client_ = 0; 
  MQTTClient_connectOptions   mqtt_conn_opts_ = MQTTClient_connectOptions_initializer;
  MQTTClient_message          mqtt_pubmsg_ = MQTTClient_message_initializer;
  MQTTClient_deliveryToken    mqtt_token_ = 0;
  MQTTClient_SSLOptions       mqtt_sslopts_ = MQTTClient_SSLOptions_initializer; 
  std::string                 mqttt_user_name = "unused";
  std::string                 mqttt_user_password_;
  int                         mqtt_keepalive_ = 60;

  const int kQos = 1;
  const unsigned long kTimeout = 10000L;

  const unsigned long kInitialConnectIntervalMillis = 500L;
  const unsigned long kMaxConnectIntervalMillis = 6000L;
  const unsigned long kMaxConnectRetryTimeElapsedMillis = 900000L;
  const float kIntervalMultiplier = 1.5f;

  std::map<std::string, std::function<bool(nlohmann::json &protocol)>> command_map_;
};
