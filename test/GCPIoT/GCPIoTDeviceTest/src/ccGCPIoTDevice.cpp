#include <iostream>
#include <fstream>

#include "ccCore/ccCoreAPI.h"
#include "ccCore/ccString.h"
#include "ccCore/ccTimer.h"

#include "ccGCPIoTDevice.h"
#include "ccJWTManager.h"

using namespace Luna;

ccGCPIoTDevice::ccGCPIoTDevice(const std::string &strSpecFile)
    : is_connected_(false), is_stop_by_user_(false), mqtt_client_(0) {

  loadSpecFile(strSpecFile);

  //  Set command functions
  command_map_["SetDevice"] = std::bind(&ccGCPIoTDevice::set_device_command,
                                        this, std::placeholders::_1);
  command_map_["GetDeviceStatus"] = std::bind(
      &ccGCPIoTDevice::get_device_status_command, this, std::placeholders::_1);
}

ccGCPIoTDevice::~ccGCPIoTDevice() { stop(); }

bool ccGCPIoTDevice::loadSpecFile(const std::string& file) {
  //  Load environment file to interwork with GCP IoT Core
  std::ifstream json_file(file);
  json_file >> device_spec_;

  std::string clientid;

  ccString::format(
    clientid, 
    "projects/%s/locations/%s/registries/%s/devices/%s",
    device_spec_["projectid"].get<std::string>().c_str(),
    device_spec_["locations"].get<std::string>().c_str(),
    device_spec_["registryid"].get<std::string>().c_str(),
    device_spec_["deviceid"].get<std::string>().c_str());

  device_spec_["clientid"] = clientid;

  return true;
}

bool ccGCPIoTDevice::initMqttClient() {

  if (mqtt_client_ != 0)
    clearMqttClient();

  mqtt_conn_opts_ = MQTTClient_connectOptions_initializer;
  mqtt_pubmsg_ = MQTTClient_message_initializer;
  mqtt_token_ = {0};

  MQTTClient_create(&mqtt_client_,
                    device_spec_["endpoint"].get<std::string>().c_str(),
                    device_spec_["clientid"].get<std::string>().c_str(),
                    MQTTCLIENT_PERSISTENCE_NONE,
                    NULL);


  mqttt_user_password_ = ccJWTManager::createJWT(device_spec_);

  mqtt_conn_opts_.keepAliveInterval = mqtt_keepalive_;
  mqtt_conn_opts_.cleansession = 1;
  mqtt_conn_opts_.username = mqttt_user_name.c_str();
  mqtt_conn_opts_.password = mqttt_user_password_.c_str();  

  mqtt_sslopts_= MQTTClient_SSLOptions_initializer;

  mqtt_sslopts_.trustStore = device_spec_["rootpath"].get<std::string>().c_str();
  mqtt_sslopts_.privateKey = device_spec_["ecpath"].get<std::string>().c_str();

  mqtt_conn_opts_.ssl = &mqtt_sslopts_;  

  return true;
}

bool ccGCPIoTDevice::clearMqttClient() {
  if (mqtt_client_ == 0)
    return false;

  MQTTClient_disconnect(mqtt_client_, 10000);
  MQTTClient_destroy(&mqtt_client_);    

  mqtt_client_ = 0;

  return true;
}

bool ccGCPIoTDevice::sendMqttTopic(const std::string& payload) {
  if (mqtt_client_ == 0)
    return false;
    
  mqtt_pubmsg_.payload = (void*)payload.c_str();
  mqtt_pubmsg_.payloadlen = payload.length();
  mqtt_pubmsg_.qos = kQos;
  mqtt_pubmsg_.retained = 0;

  MQTTClient_publishMessage(mqtt_client_, device_spec_["topic"].get<std::string>().c_str(), &mqtt_pubmsg_, &mqtt_token_);

  //printf("Waiting for up to %lu seconds for publication of %s\n"
  //        "on topic %s for client with ClientID: %s\n",
  //        (kTimeout/1000), device_spec_["endpoint"].get<std::string>().c_str(), device_spec_["topic"].get<std::string>().c_str(), device_spec_["clientid"].get<std::string>().c_str();
  
  int rc = MQTTClient_waitForCompletion(mqtt_client_, mqtt_token_, kTimeout);
  
  std::cout << "Message with delivery token " << mqtt_token_ << " delivered" << std::endl;

  return true;
}

bool ccGCPIoTDevice::start() {
  if (is_connected_) return true;

  is_stop_by_user_ = false;

  int rc = -1;

  unsigned long retry_interval_ms = kInitialConnectIntervalMillis;
  unsigned long total_retry_time_ms = 0;

  std::cout << "I'm ready to connect to server!" << std::endl;

  initMqttClient();

  while ((rc = MQTTClient_connect(mqtt_client_, &mqtt_conn_opts_)) !=
         MQTTCLIENT_SUCCESS) {
    if (rc == 3) {  // connection refused: server unavailable
      usleep(retry_interval_ms / 1000);

      total_retry_time_ms += retry_interval_ms;

      if (total_retry_time_ms >= kMaxConnectRetryTimeElapsedMillis) {
        printf("Failed to connect, maximum retry time exceeded.");
        exit(EXIT_FAILURE);

        std::cout << "Failed to connect, maximum retry time exceeded. " << std::endl;
        return false;        
      }

      retry_interval_ms *= kIntervalMultiplier;
      
      if (retry_interval_ms > kMaxConnectIntervalMillis) {
        retry_interval_ms = kMaxConnectIntervalMillis;
      }

    } else {
      std::cout << "Failed to connect, return code: " << rc << std::endl;
      return false;
    }
  }

  return true;
}

bool ccGCPIoTDevice::stop() {
  is_stop_by_user_ = true;

  if (!is_connected_) return true;

  is_connected_ = false;

  clearMqttClient();

  return true;
}

bool ccGCPIoTDevice::send(const nlohmann::json &protocol) {
  //  return protocol.send(&ws_client_);
  return false;
}

bool ccGCPIoTDevice::set_device_command(nlohmann::json &protocol) { return true; }

bool ccGCPIoTDevice::get_device_status_command(nlohmann::json &protocol) {
  return true;
}
