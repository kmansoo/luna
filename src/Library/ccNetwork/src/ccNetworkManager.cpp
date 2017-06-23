/*
 * ccNetworkManager.cpp
 *
 *  Created on: 2016. 1. 27.
 *      Author: kmansoo
 */
#include <iostream>
#include <string.h>

#if defined(_WINDOWS)
#   include <Winsock2.h>
#   include <IPHlpApi.h>
#   pragma comment(lib, "ws2_32.lib")
#   pragma comment(lib, "IPHLPAPI.lib")
#else
#   include <netdb.h>
#   include <sys/socket.h>       /* for AF_INET */
#   include <unistd.h>
#   include <arpa/inet.h>

#   define  SOCKET_ERROR    -1
#endif

#include "ccCore/ccString.h"

#include "ccNetwork/ccNetworkManager.h"

namespace Luna {

ccNetworkManager::ccNetworkManager() {
    is_init_network_ = false;
}

ccNetworkManager::~ccNetworkManager() {
    shutdown();
}

bool ccNetworkManager::init() {
    if (is_init_network_)
        return true;

#if defined(_WINDOWS)
    WORD version_requested = MAKEWORD(2, 2);
    WSADATA wsa_data;
    int err;

    err = WSAStartup(version_requested, &wsa_data);

    if (err != 0) {
        //// could not find a usable WinSock DLL
        ////cerr << "Could not load winsock" << endl;
        // assert(0); // is this is failing, try a different version that 2.2, 1.0
        // or later will likely work
        // exit(1);
    } else
        is_init_network_ = true;
#endif

    return true;
}

void ccNetworkManager::shutdown() {
#if defined(_WINDOWS)
    if (is_init_network_) {
        WSACleanup();

        is_init_network_ = false;
    }
#endif
}

bool ccNetworkManager::get_host_name(std::string& strName) {
    char buffer[1024];

    if (::gethostname(buffer, sizeof(buffer)) == SOCKET_ERROR)
        return false;

    strName = buffer;

    return true;
}

bool ccNetworkManager::get_local_ip(std::string& ip) {
    std::string host_name;

    if (get_host_name(host_name) == false)
        return false;

    struct hostent* host = ::gethostbyname((char*)host_name.c_str());

    if (host == NULL)
        return false;

    //  Obtain the computer's IP
#if defined(_WINDOWS)
    ccString::format(ip, "%d.%d.%d.%d",
                     ((struct in_addr*)(host->h_addr))->S_un.S_un_b.s_b1,
                     ((struct in_addr*)(host->h_addr))->S_un.S_un_b.s_b2,
                     ((struct in_addr*)(host->h_addr))->S_un.S_un_b.s_b3,
                     ((struct in_addr*)(host->h_addr))->S_un.S_un_b.s_b4);
#else
    ip = inet_ntoa(*(struct in_addr*)*host->h_addr_list);
#endif

    return true;
}

bool ccNetworkManager::get_subnet_mask(std::string& ip) {
    return false;
}

bool ccNetworkManager::get_gateway_ip(std::string& ip) {
    char host_name[80];

    if (::gethostname(host_name, sizeof(host_name)) == SOCKET_ERROR)
        return false;

    struct hostent* host_info = ::gethostbyname(host_name);

    if (host_info == 0) {
        //  std::cout << "Yow! Bad host lookup." << std::endl;
        return false;
    }

    for (int nIndex = 0; host_info->h_addr_list[nIndex] != 0; ++nIndex) {
        struct in_addr addr;
        memcpy(&addr, host_info->h_addr_list[nIndex], sizeof(struct in_addr));

        ip = inet_ntoa(addr);

        break;
        //  std::cout << "Address " << nIndex << ": " << inet_ntoa(addr) <<
        //  std::endl;
    }

    return true;
}

std::uint32_t ccNetworkManager::convert_ip_to_int(const std::string& ip) {
    std::uint32_t converted_ip = 0;
    static const char digits[] = "0123456789";

    std::uint8_t tmp[4];
    std::uint8_t* tp = NULL;

    int saw_digit = 0;
    int octets = 0;
    *(tp = tmp) = 0;

    for (auto item : ip) {
        const char* pch;

        if ((pch = strchr(digits, item)) != NULL) {
            std::uint32_t newVal = *tp * 10 + (pch - digits);

            if (newVal > 255)
                return (0);

            *tp = newVal;

            if (!saw_digit) {
                if (++octets > 4)
                    return (0);

                saw_digit = 1;
            }
        } else {
            if (item == '.' && saw_digit) {
                if (octets == 4)
                    return (0);

                *++tp = 0;
                saw_digit = 0;
            } else
                return (0);
        }
    }

    if (octets < 4)
        return (0);

    converted_ip = (((std::uint32_t)tmp[0] << 24) & 0xFF000000);
    converted_ip |= (((std::uint32_t)tmp[1] << 16) & 0x00FF0000);
    converted_ip |= (((std::uint32_t)tmp[2] << 8) & 0x0000FF00);
    converted_ip |= (tmp[3] & 0xFF);

    return converted_ip;
}

bool ccNetworkManager::convert_int_to_ip(std::uint32_t uIP, std::string& ip) {
    ccString::format(ip, "%d.%d.%d.%d", (uIP >> 24) & 0xFF, (uIP >> 16) & 0xFF,
                     (uIP >> 8) & 0xFF, uIP & 0xFF);

    return true;
}

bool ccNetworkManager::is_public_ip(const std::string& ip) {
    char number[3];

    if (ip.length() > 0) {
        //  HSPLog("[NetInf] IsPublicIP : %s", ip.c_str());
    }

    if (ip.find("10.") == 0 || ip.find("192.168.") == 0)
        return false;
    else {
        if (ip.find("172.") == 0) {
            strncpy(number, ip.c_str() + 4, 3);

            if (number[2] != '.')
                return true;

            number[2] = 0x00;

            int int_value = atoi(number);

            if (int_value >= 16 && int_value <= 31)
                return false;
        }
    }

    return true;
}

}
