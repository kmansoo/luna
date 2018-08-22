//
//  https://github.com/cursey/jwt.git
//

#pragma once

#include <chrono>
#include <cstdint>
#include <set>
#include <string>

#include "nlohmann/json.hpp"

namespace jwt {
//
// JWT encoding/decoding functions.
//

// Returns an empty string on failure.
std::string encode(const nlohmann::json& payload, const std::string& key,
                   const std::string& alg = "");

// Returns a null json object on failure. Doesn't throw.
nlohmann::json decode(const std::string& jwt, const std::string& key,
                      const std::set<std::string>& alg = {});

//
// Helper functions to add claims to a payload.
//

// Adds an issuer claim ("iss").
void issuedBy(nlohmann::json& payload, const std::string& issuer);

// Adds a subject claim ("sub").
void subjectOf(nlohmann::json& payload, const std::string& subject);

// Adds an audience claim ("aud").
void issuedFor(nlohmann::json& payload, const std::set<std::string>& audience);

// Adds an expiration time claim ("exp").
void expiresAt(nlohmann::json& payload, std::time_t time);

// Adds a not before claim ("nbf").
void useAfter(nlohmann::json& payload,
              std::time_t time = std::chrono::system_clock::to_time_t(
                  std::chrono::system_clock::now()));

// Adds an issued at claim ("iat").
void issuedAt(nlohmann::json& payload,
              std::time_t time = std::chrono::system_clock::to_time_t(
                  std::chrono::system_clock::now()));

// Adds a JWT ID claim ("jti").
void identifiesAs(nlohmann::json& payload, const std::string& id);

// Helper functions to verify claims made by a payload.
namespace claims {
enum Flags : unsigned int {
  NONE = 0,
  ISS = 1 << 0,  // Issuer claim.
  SUB = 1 << 1,  // Subject claim.
  AUD = 1 << 2,  // Audience claim.
  EXP = 1 << 3,  // Expiration time claim.
  NBF = 1 << 4,  // Not before claim.
  IAT = 1 << 5,  // Issued at claim.
  JTI = 1 << 6,  // JWT ID claim.
  EVERYTHING = ISS | SUB | AUD | EXP | NBF | IAT | JTI
};
}

struct AcceptedParameters {
  std::set<std::string> issuers;
  std::set<std::string> subjects;
  std::set<std::string> audience;
  std::set<std::string>
      usedIDs;  // ID's that will cause the JWT ID claim to fail.
  std::time_t now{
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())};
};

// Doesn't throw.
bool verify(const nlohmann::json& payload, unsigned int claims,
            const AcceptedParameters& params = {});
}  // namespace jwt