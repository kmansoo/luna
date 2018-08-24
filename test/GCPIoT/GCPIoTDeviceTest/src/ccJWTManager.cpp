#include <iostream>
#include <fstream>
#include <streambuf>

#include "ccCore/ccString.h"

#include "JWT/jwt.hpp"

#include "ccJWTManager.h"

std::string ccJWTManager::createJWT(nlohmann::json& device_spec, const std::string& algorithm) {
  std::string iat_time, exp_time;

  time_t now_seconds = time(NULL);

  Luna::ccString::format(iat_time, "%lu", now_seconds);
  Luna::ccString::format(exp_time, "%lu", now_seconds + 3600);

  nlohmann::json jwt_payload{
      { "aud", device_spec["projectid"].get<std::string>()},
      { "iat", iat_time },
      { "exp", exp_time }
  }; 

  // Read private key from file
  std::ifstream file(device_spec["ecpath"].get<std::string>());
  std::string private_key((std::istreambuf_iterator<char>(file)),
                  std::istreambuf_iterator<char>());  

  auto json_web_token = jwt::encode(jwt_payload, private_key, algorithm);

  return json_web_token;
}
