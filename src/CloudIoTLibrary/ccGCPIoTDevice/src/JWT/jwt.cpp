#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <vector>

#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/pem.h>

#include "jwt.hpp"

using namespace std;
using namespace nlohmann;

namespace jwt {
namespace detail {
class OnLeave {
 public:
  OnLeave() = default;

  ~OnLeave() {
    for (auto& fn : m_callbacks) {
      fn();
    }
  }

  void add(function<void()> fn) { m_callbacks.emplace_back(move(fn)); }

 private:
  vector<function<void()>> m_callbacks;
};

void replaceAll(string& str, const string& from, const string& to) {
  size_t pos = 0;

  while ((pos = str.find(from, pos)) != string::npos) {
    str.replace(pos, from.length(), to);
    pos += to.length();
  }
}

string b64encode(const uint8_t* data, size_t len) {
  auto b64 = BIO_new(BIO_f_base64());
  auto bio = BIO_new(BIO_s_mem());

  bio = BIO_push(b64, bio);

  BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
  BIO_write(bio, data, (int)len);
  BIO_flush(bio);

  BUF_MEM* buf = nullptr;

  BIO_get_mem_ptr(bio, &buf);

  string s(buf->data, buf->length);

  BIO_free_all(bio);

  // Convert it to base64url.
  s = s.substr(0, s.find_last_not_of('=') + 1);
  replaceAll(s, "+", "-");
  replaceAll(s, "/", "_");

  return s;
}

vector<uint8_t> b64decode(string str) {
  // Convert it from base64url back to normal base64.
  size_t padding{0};

  replaceAll(str, "-", "+");
  replaceAll(str, "_", "/");

  switch (str.length() % 4) {
    case 0:
      break;

    case 2:
      str += "==";
      padding = 2;
      break;

    case 3:
      str += "=";
      padding = 1;
      break;

    default:
      return vector<uint8_t>{};
  }

  size_t len{(str.length() * 3) / 4 - padding};
  vector<uint8_t> buf(len);

  auto bio = BIO_new_mem_buf((void*)str.c_str(), -1);
  auto b64 = BIO_new(BIO_f_base64());

  bio = BIO_push(b64, bio);

  BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
  BIO_read(bio, buf.data(), (int)str.length());
  BIO_free_all(bio);

  return buf;
}
}  // namespace detail

#define SCOPE_EXIT(x)          \
  do {                         \
    onLeave.add([&]() { x; }); \
  } while (0)

string signHMAC(const string& str, const string& key, const string& alg) {
  const EVP_MD* evp = nullptr;

  if (alg == "HS256") {
    evp = EVP_sha256();
  } else if (alg == "HS384") {
    evp = EVP_sha384();
  } else if (alg == "HS512") {
    evp = EVP_sha512();
  } else {
    return string{};
  }

  vector<uint8_t> out(EVP_MAX_MD_SIZE);
  unsigned int len = 0;

  HMAC(evp, key.c_str(), (int)key.length(), (const unsigned char*)str.c_str(),
       str.length(), out.data(), &len);

  return detail::b64encode(out.data(), len);
}

string signPEM(const string& str, const string& key, const string& alg) {
  detail::OnLeave onLeave{};
  const EVP_MD* evp = nullptr;

  if (alg == "RS256") {
    evp = EVP_sha256();
  } else if (alg == "RS384") {
    evp = EVP_sha384();
  } else if (alg == "RS512") {
    evp = EVP_sha512();
  } else if (alg == "ES256") {
    evp = EVP_sha256();
  } else if (alg == "ES384") {
    evp = EVP_sha384();
  } else if (alg == "ES512") {
    evp = EVP_sha512();
  } else {
    return {};
  }

  auto bufkey = BIO_new_mem_buf((void*)key.c_str(), (int)key.length());
  SCOPE_EXIT(if (bufkey != nullptr) BIO_free(bufkey));

  if (bufkey == nullptr) {
    return {};
  }

  // Use OpenSSL's default passphrase callbacks if needed.
  auto pkey = PEM_read_bio_PrivateKey(bufkey, nullptr, nullptr, nullptr);
  SCOPE_EXIT(if (pkey != nullptr) EVP_PKEY_free(pkey));

  if (pkey == nullptr) {
    return {};
  }

  auto mdctx = EVP_MD_CTX_create();
  SCOPE_EXIT(if (mdctx != nullptr) EVP_MD_CTX_destroy(mdctx));

  if (mdctx == nullptr) {
    return {};
  }

  // Initialize the digest sign operation.
  if (EVP_DigestSignInit(mdctx, nullptr, evp, nullptr, pkey) != 1) {
    return {};
  }

  // Update the digest sign with the message.
  if (EVP_DigestSignUpdate(mdctx, str.c_str(), str.length()) != 1) {
    return {};
  }

  // Determin the size of the finalized digest sign.
  size_t siglen = 0;

  if (EVP_DigestSignFinal(mdctx, nullptr, &siglen) != 1) {
    return {};
  }

  // Finalize it.
  vector<uint8_t> sig(siglen);

  if (EVP_DigestSignFinal(mdctx, sig.data(), &siglen) != 1) {
    return {};
  }

  // For RSA, we are done.
  return detail::b64encode(sig.data(), siglen);
}

bool verifyPEM(const string& str, const string& b64sig, const string& key,
               const string& alg) {
  detail::OnLeave onLeave{};
  const EVP_MD* evp = nullptr;

  if (alg == "RS256") {
    evp = EVP_sha256();
  } else if (alg == "RS384") {
    evp = EVP_sha384();
  } else if (alg == "RS512") {
    evp = EVP_sha512();
  } else if (alg == "ES256") {
    evp = EVP_sha256();
  } else if (alg == "ES384") {
    evp = EVP_sha384();
  } else if (alg == "ES512") {
    evp = EVP_sha512();
  } else {
    return false;
  }

  auto sig = detail::b64decode(b64sig);
  auto siglen = sig.size();

  if (sig.empty()) {
    return false;
  }

  auto bufkey = BIO_new_mem_buf((void*)key.c_str(), (int)key.length());
  SCOPE_EXIT(if (bufkey != nullptr) BIO_free(bufkey));

  if (bufkey == nullptr) {
    return false;
  }

  // Use OpenSSL's default passphrase callbacks if needed.
  auto pkey = PEM_read_bio_PUBKEY(bufkey, nullptr, nullptr, nullptr);
  SCOPE_EXIT(if (pkey != nullptr) EVP_PKEY_free(pkey));

  if (pkey == nullptr) {
    return false;
  }

  auto mdctx = EVP_MD_CTX_create();
  SCOPE_EXIT(if (mdctx != nullptr) EVP_MD_CTX_destroy(mdctx));

  if (EVP_DigestVerifyInit(mdctx, nullptr, evp, nullptr, pkey) != 1) {
    return false;
  }

  if (EVP_DigestVerifyUpdate(mdctx, str.c_str(), str.length()) != 1) {
    return false;
  }

  if (EVP_DigestVerifyFinal(mdctx, sig.data(), siglen) != 1) {
    return false;
  }

  return true;
}

string encode(const json& payload, const string& key, const string& alg) {
  // Create a JWT header defaulting to the HS256 alg if none is supplied.
  json header{{"typ", "JWT"}, {"alg", alg.empty() ? "HS256" : alg}};
  auto headerStr = header.dump();
  auto encodedHeader =
      detail::b64encode((const uint8_t*)headerStr.c_str(), headerStr.length());

  // Encode the payload.
  auto payloadStr = payload.dump();
  auto encodedPayload = detail::b64encode((const uint8_t*)payloadStr.c_str(),
                                          payloadStr.length());

  // Sign it and return the final JWT.
  auto encodedToken = encodedHeader + "." + encodedPayload;
  const string& theAlg = header["alg"];
  string signature{};

  if (theAlg == "none") {
    // Nothing to sign.
  } else if (theAlg.find("HS") != string::npos) {
    signature = signHMAC(encodedToken, key, theAlg);
  } else {
    signature = signPEM(encodedToken, key, theAlg);
  }

  if (theAlg != "none" && signature.empty()) {
    return {};
  }

  return encodedToken + "." + signature;
}

json decode(const string& jwt, const string& key, const set<string>& alg) {
  try {
    if (jwt.empty()) {
      return {};
    }

    // Make sure the jwt we recieve looks like a jwt.
    auto firstPeriod = jwt.find_first_of('.');
    auto secondPeriod = jwt.find_first_of('.', firstPeriod + 1);

    if (firstPeriod == string::npos || secondPeriod == string::npos) {
      return {};
    }

    // Decode the header so we can get the alg used by the jwt.
    auto decodedHeader = detail::b64decode(jwt.substr(0, firstPeriod));
    string decodedHeaderStr{decodedHeader.begin(), decodedHeader.end()};
    auto header = json::parse(decodedHeaderStr.c_str());
    const string& theAlg = header["alg"];

    // Make sure no key is supplied if the alg is none.
    if (theAlg == "none" && !key.empty()) {
      return {};
    }

    // Make sure the alg supplied is one we expect.
    if (alg.count(theAlg) == 0 && !alg.empty()) {
      return {};
    }

    auto encodedToken = jwt.substr(0, secondPeriod);
    auto signature = jwt.substr(secondPeriod + 1);

    // Verify the signature.
    if (theAlg == "none") {
      // Nothing to do, no verification needed.
    } else if (theAlg.find("HS") != string::npos) {
      auto calculatedSignature = signHMAC(encodedToken, key, theAlg);

      if (signature != calculatedSignature || calculatedSignature.empty()) {
        return {};
      }
    } else {
      if (!verifyPEM(encodedToken, signature, key, theAlg)) {
        return {};
      }
    }

    // Decode the payload since the jwt has been verified.
    auto decodedPayload = detail::b64decode(
        jwt.substr(firstPeriod + 1, secondPeriod - firstPeriod - 1));
    string decodedPayloadStr{decodedPayload.begin(), decodedPayload.end()};
    auto payload = json::parse(decodedPayloadStr.c_str());

    return payload;
  } catch (...) {
    return {};
  }
}

void issuedBy(json& payload, const string& issuer) { payload["iss"] = issuer; }

void subjectOf(json& payload, const string& subject) {
  payload["sub"] = subject;
}

void issuedFor(json& payload, const set<string>& audience) {
  if (audience.size() == 1) {
    payload["aud"] = (*audience.cbegin());
  } else {
    for (const auto& member : audience) {
      payload["aud"].push_back(member);
    }
  }
}

void expiresAt(json& payload, time_t time) { payload["exp"] = time; }

void useAfter(json& payload, time_t time) { payload["nbf"] = time; }

void issuedAt(json& payload, time_t time) { payload["iat"] = time; }

void identifiesAs(json& payload, const string& id) { payload["jti"] = id; }

bool verify(const json& payload, unsigned int claims,
            const AcceptedParameters& params) {
  try {
    // json::value<> can only be used on objects, and payloads should always
    // be objects.
    if (!payload.is_object()) {
      return false;
    }

    bool isValid{true};

    // Check the issuer claim.
    if ((claims & claims::ISS) != 0) {
      auto iss = payload.at("iss").get<string>();

      if (params.issuers.count(iss) == 0) {
        isValid = false;
      }
    }

    // Check the subject claim.
    if ((claims & claims::SUB) != 0) {
      auto sub = payload.at("sub").get<string>();

      if (params.subjects.count(sub) == 0) {
        isValid = false;
      }
    }

    // Check the audience claim.
    if ((claims & claims::AUD) != 0) {
      auto aud = payload.at("aud");

      if (aud.is_array()) {
        auto audience = aud.get<set<string>>();
        vector<string> intersection{};

        set_intersection(audience.begin(), audience.end(),
                         params.audience.begin(), params.audience.end(),
                         back_inserter(intersection));

        if (intersection.empty()) {
          isValid = false;
        }
      } else {
        if (params.audience.count(aud) == 0) {
          isValid = false;
        }
      }
    }

    // Check the expires at claim.
    if ((claims & claims::EXP) != 0) {
      auto exp = payload.at("exp").get<time_t>();

      if (params.now > exp) {
        isValid = false;
      }
    }

    // Check the not before claim.
    if ((claims & claims::NBF) != 0) {
      auto nbf = payload.at("nbf").get<time_t>();

      if (params.now < nbf) {
        isValid = false;
      }
    }

    // Issued at claim is just for checking the age of the jwt, just check
    // that there is an iat entry with a resonable value.
    if ((claims & claims::IAT) != 0) {
      auto iat = payload.at("iat");

      if (!iat.is_number_unsigned()) {
        isValid = false;
      }
    }

    // Check the JWT ID claim.
    if ((claims & claims::JTI) != 0) {
      auto jti = payload.at("jti").get<string>();

      if (params.usedIDs.count(jti) != 0) {
        isValid = false;
      }
    }

    return isValid;
  } catch (...) {
    // If there was a problem verifying something, then it seems resonable
    // to return false.
    return false;
  }
}
}  // namespace jwt