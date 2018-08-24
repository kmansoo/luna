#include <iostream>
#include <fstream>

#include "ccCore/ccCoreAPI.h"
#include "ccCore/ccString.h"
#include "ccCore/ccTimer.h"

#include "ccGCPIoTDevice.h"

#include "JWT/jwt.hpp"

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
  std::string topic;

  ccString::format(
    clientid, 
    "projects/%s/locations/%s/registries/%s/devices/%s",
    device_spec_["projectid"].get<std::string>().c_str(),
    device_spec_["locations"].get<std::string>().c_str(),
    device_spec_["registryid"].get<std::string>().c_str(),
    device_spec_["deviceid"].get<std::string>().c_str());

  ccString::format(
    topic, 
    "/devices/%s/state", //  "/devices/%s/events",
    device_spec_["deviceid"].get<std::string>().c_str());    

  device_spec_["clientid"] = clientid;
  device_spec_["topic"] = topic;

  return true;
}

bool ccGCPIoTDevice::initMqttClient() {
  std::cout << "initMqttClient, enter" << std::endl;

  if (mqtt_client_ != 0)
    clearMqttClient();

  mqtt_conn_opts_ = MQTTClient_connectOptions_initializer;
  mqtt_pubmsg_ = MQTTClient_message_initializer;
  mqtt_token_ = {0};

  MQTTClient_create(&mqtt_client_,
                    device_spec_["endpoint"].get<std::string>().c_str(),
                    device_spec_["clientid"].get<std::string>().c_str(),
                    MQTTCLIENT_PERSISTENCE_DEFAULT,
                    NULL);

  mqttt_user_password_ = createJWT();

  mqtt_conn_opts_.keepAliveInterval = mqtt_keepalive_;
  mqtt_conn_opts_.cleansession = 1;
  mqtt_conn_opts_.username = mqttt_user_name.c_str();
  mqtt_conn_opts_.password = mqttt_user_password_.c_str();  

  MQTTClient_setCallbacks(mqtt_client_, this, &ccGCPIoTDevice::on_mqtt_disconnect, &ccGCPIoTDevice::on_mqtt_receive_message, NULL);

  return true;
}

void ccGCPIoTDevice::on_mqtt_disconnect(void* context, char* cause) {
  std::cout << "on_mqtt_disconnect, enter" << std::endl;

  if (cause != NULL)
    std::cout << "cause: " << cause << std::endl;
  std::cout << "on_mqtt_disconnect, leave" << std::endl;
}

int ccGCPIoTDevice::ccGCPIoTDevice::on_mqtt_receive_message(void* context, char* topicName, int topicLen, MQTTClient_message* message) {
  /* 
    if 0 (false) is returned by the callback then it failed, so we don't remove the message from
    the queue, and it will be retried later.  If 1 is returned then the message data may have been freed,
    so we must be careful how we use it.
  */
    
  std::cout << "on_mqtt_receive_message, enter" << std::endl;

  std::cout << "topicName: " << topicName << std::endl;

  if (message->payloadlen > 0) {
    std::string payload((char*)message->payload, message->payloadlen);

    std::cout << "payloadlen: " << message->payloadlen << std::endl;
    std::cout << "message: " << payload << std::endl;
  }

  std::cout << "on_mqtt_receive_message, leave" << std::endl;

  return 1;
}

bool ccGCPIoTDevice::clearMqttClient() {
  std::cout << "clearMqttClient, enter" << std::endl;

  if (mqtt_client_ == 0)
    return false;

  MQTTClient_disconnect(mqtt_client_, 10000);
  MQTTClient_destroy(&mqtt_client_);    

  mqtt_client_ = 0;

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

  // Set up for SSL
  MQTTClient_SSLOptions sslopts = MQTTClient_SSLOptions_initializer;

  sslopts.trustStore = device_spec_["rootpath"].get<std::string>().c_str();
  sslopts.privateKey = device_spec_["ecpath"].get<std::string>().c_str();
  
  mqtt_conn_opts_.ssl = &sslopts;    

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

  //  Subscribe Test
  std::string topic;

  ccString::format(
    topic, 
    "/devices/%s/config",
    device_spec_["deviceid"].get<std::string>().c_str());   

  std::cout << "Subscribe to " << topic << std::endl;

  MQTTClient_subscribe(mqtt_client_, topic.c_str(), 1);

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

bool ccGCPIoTDevice::sendMqttTopic(const std::string& topic_name, const std::string& payload) {
  if (mqtt_client_ == 0)
    return false;

  std::string topic;

  ccString::format(
    topic, 
    "/devices/%s/%s", //  "/devices/%s/events",
    device_spec_["deviceid"].get<std::string>().c_str(),
    topic_name.c_str());    

  std::cout << "topic: " << topic << std::endl;
  std::cout << "payload: " << payload << std::endl;

  mqtt_pubmsg_.payload = (void*)payload.c_str();
  mqtt_pubmsg_.payloadlen = payload.length();
  mqtt_pubmsg_.qos = kQos;
  mqtt_pubmsg_.retained = 0;

  MQTTClient_deliveryToken mqtt_token_ = 0;

  MQTTClient_publishMessage(mqtt_client_, topic.c_str(), &mqtt_pubmsg_, &mqtt_token_);

  int rc = MQTTClient_waitForCompletion(mqtt_client_, mqtt_token_, kTimeout);
  
  std::cout << "Message with delivery token " << mqtt_token_ << " delivered" << std::endl;

  return true;
}

std::string ccGCPIoTDevice::createJWT(const std::string& algorithm) {
  std::string iat_time, exp_time;

  time_t now_seconds = time(NULL);

  Luna::ccString::format(iat_time, "%lu", now_seconds);
  Luna::ccString::format(exp_time, "%lu", now_seconds + 3600);

  nlohmann::json jwt_payload{
      { "aud", device_spec_["projectid"]},
      { "iat", iat_time },
      { "exp", exp_time }
  }; 

  // Read private key from file
  std::ifstream file(device_spec_["ecpath"].get<std::string>());
  std::string private_key((std::istreambuf_iterator<char>(file)),
                  std::istreambuf_iterator<char>());  

  auto json_web_token = jwt::encode(jwt_payload, private_key, algorithm);

  return json_web_token;
}
