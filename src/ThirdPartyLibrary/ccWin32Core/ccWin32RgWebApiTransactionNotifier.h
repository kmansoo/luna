#pragma once

#include <string>

namespace Luna {

class ccWin32RgWebApiTransactionNotifier
{
public:
    virtual void    onTransactionRecveResponse(std::uint16_t status_code, const std::string& response) = 0;
    virtual void    onTransactionRequestTimeout() = 0;
};

}