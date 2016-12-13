/*
 * ccNetworkManager.h
 *
 *  Created on: 2016. 1. 27.
 *      Author: kmansoo
 */

#ifndef LIBRARY_CCNETWORKAPI_CCNETWORKMANAGER_H_
#define LIBRARY_CCNETWORKAPI_CCNETWORKMANAGER_H_

#include <string>

#include "ccCore/ccSingleton.h"

namespace Luna {

class ccNetworkManager : public ccSingleton<ccNetworkManager> {
public:
    ccNetworkManager();
    virtual ~ccNetworkManager();

public:
    bool    init();
    void    shutdown();

    bool    get_local_ip(std::string& ip);
    bool    get_subnet_mask(std::string& mask);
    bool    get_gateway_ip(std::string& ip);

    std::uint32_t   convert_ip_to_int(const std::string& ip);
    bool            convert_int_to_ip(std::uint32_t uIP, std::string& ip);
    bool            is_public_ip(const std::string& ip);

    bool            get_host_name(std::string& strName);

private:
    bool    is_init_network_;
};

}

#endif /* LIBRARY_CCNETWORKAPI_CCNETWORKMANAGER_H_ */
