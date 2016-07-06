#pragma once

#include <string>

namespace Luna {

class ccWin32RgWebApiTransactionNotifier
{
public:
    virtual void    onTransactionRecveResponse(std::uint16_t uStatusCode, const std::string& strResponse) = 0;
    virtual void    onTransactionRequestTimeout() = 0;
};

}