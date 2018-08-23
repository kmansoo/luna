#include <iostream>
#include <fstream>

#include "ccCore/ccCoreAPI.h"
#include "ccCore/ccTimer.h"

#include "ccGCPIoTDevice.h"

#include "JWT/jwt.hpp"

#include "ccMqttClient/MQTTClient.h"

using namespace Luna;

ccGCPIoTDevice::ccGCPIoTDevice(const std::string &strSpecFile)
    : is_connected_(false), is_stop_by_user_(false) {

  std::ifstream json_file(strSpecFile);

  json_file >> device_spec_;

  command_map_["SetDevice"] = std::bind(&ccGCPIoTDevice::set_device_command,
                                        this, std::placeholders::_1);
  command_map_["GetDeviceStatus"] = std::bind(
      &ccGCPIoTDevice::get_device_status_command, this, std::placeholders::_1);

  std::cout << "enter ccGCPIoTDevice::ccGCPIoTDevice()" << std::endl;

}

ccGCPIoTDevice::~ccGCPIoTDevice() { stop(); }


// [START iot_mqtt_jwt]
/**
 * Calculates issued at / expiration times for JWT and places the time, as a
 * Unix timestamp, in the strings passed to the function. The time_size
 * parameter specifies the length of the string allocated for both iat and exp.
 */
static void GetIatExp(char* iat, char* exp, int time_size) {
  // TODO: Use time.google.com for iat
  time_t now_seconds = time(NULL);
  snprintf(iat, time_size, "%lu", now_seconds);
  snprintf(exp, time_size, "%lu", now_seconds + 3600);
}

/**
 * Calculates a JSON Web Token (JWT) given the path to a EC private key and
 * Google Cloud project ID. Returns the JWT as a string that the caller must
 * free.
 */
std::string CreateJwt(const char *ec_private_path, const char *project_id) {
  char iat_time[sizeof(time_t) * 3 + 2];
  char exp_time[sizeof(time_t) * 3 + 2];
  uint8_t *key = NULL;  // Stores the Base64 encoded certificate
  size_t key_len = 0;
  int ret = 0;

  // Read private key from file
  FILE *fp = fopen(ec_private_path, "r");
  if (fp == (void *)NULL) {
    printf("Could not open file: %s\n", ec_private_path);
    return "";
  }
  fseek(fp, 0L, SEEK_END);
  key_len = ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  key = (uint8_t *)malloc(sizeof(uint8_t) * (key_len + 1));  // certificate length + \0

  fread(key, 1, key_len, fp);
  key[key_len] = '\0';
  fclose(fp);

  // Get JWT parts
  GetIatExp(iat_time, exp_time, sizeof(iat_time));

  //  
  nlohmann::json payload{
      { "aud", project_id},
      { "iat", iat_time },
      { "exp", exp_time }
  }; 

  auto out = jwt::encode(payload, std::string((char*)key), "ES256");

  free(key);

  return out;
}
// [END iot_mqtt_jwt]

bool ccGCPIoTDevice::start() {
  if (is_connected_) return true;

  is_stop_by_user_ = false;

  if (target_uri_ == "") target_uri_ = "ws://localhost:8000/ws_iot_deivce";

  int keepalive = 60;

  MQTTClient client = {0};
  MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
  MQTTClient_message pubmsg = MQTTClient_message_initializer;
  MQTTClient_deliveryToken token = {0};

  std::string kUsername = "unused";

  MQTTClient_create(&client,
                    device_spec_["endpoint"].get<std::string>().c_str(),
                    device_spec_["clientid"].get<std::string>().c_str(),
                    MQTTCLIENT_PERSISTENCE_NONE,
                    NULL);

  conn_opts.keepAliveInterval = 60;
  conn_opts.cleansession = 1;
  conn_opts.username = kUsername.c_str();

  auto password = CreateJwt(device_spec_["ecpath"].get<std::string>().c_str(), device_spec_["projectid"].get<std::string>().c_str());

  conn_opts.password = password.c_str();

  MQTTClient_SSLOptions sslopts = MQTTClient_SSLOptions_initializer;

  sslopts.trustStore = device_spec_["rootpath"].get<std::string>().c_str();
  sslopts.privateKey = device_spec_["ecpath"].get<std::string>().c_str();

  conn_opts.ssl = &sslopts;

  int rc = -1;
  static const unsigned long kInitialConnectIntervalMillis = 500L;
  static const unsigned long kMaxConnectIntervalMillis = 6000L;
  static const unsigned long kMaxConnectRetryTimeElapsedMillis = 900000L;
  static const float kIntervalMultiplier = 1.5f;

  unsigned long retry_interval_ms = kInitialConnectIntervalMillis;
  unsigned long total_retry_time_ms = 0;

  std::cout << "I'm ready to connect to server!" << std::endl;

  while ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
    if (rc == 3) {  // connection refused: server unavailable
      usleep(retry_interval_ms / 1000);

      total_retry_time_ms += retry_interval_ms;

      if (total_retry_time_ms >= kMaxConnectRetryTimeElapsedMillis) {
        printf("Failed to connect, maximum retry time exceeded.");
        exit(EXIT_FAILURE);
      }
      retry_interval_ms *= kIntervalMultiplier;
      if (retry_interval_ms > kMaxConnectIntervalMillis) {
        retry_interval_ms = kMaxConnectIntervalMillis;
      }
    } else {
      printf("Failed to connect, return code %d\n", rc);
      exit(EXIT_FAILURE);
    }
  }

  //  std::cout << "ret_value: " << ret_value << std::endl;

  return true;
}

bool ccGCPIoTDevice::stop() {
  is_stop_by_user_ = true;

  if (!is_connected_) return true;

  is_connected_ = false;

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
