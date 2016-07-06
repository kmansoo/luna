#pragma once

#include <string>

namespace Luna {

class ccWin32RgWebApiTransactionNotifier
{
public:
    virtual void    OnTransactionRecveResponse(std::uint16_t uStatusCode, const std::string& strResponse) = 0;
    virtual void    OnTransactionRequestTimeout() = 0;
};

}