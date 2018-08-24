#pragma once

#include <string>
#include "nlohmann/json.hpp"

class ccJWTManager {
public:
  static std::string createJWT(nlohmann::json& device_spec, const std::string& algorithm = "RS256");
};