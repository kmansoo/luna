/*
 * ccNetworkManager.cpp
 *
 *  Created on: 2016. 1. 27.
 *      Author: kmansoo
 */
#include <iostream>
#include <string.h>

#if defined(WIN32)
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

#include "ccNetworkManager.h"

ccNetworkManager::ccNetworkManager()
{
    // TODO Auto-generated constructor stub

    _bIsInitNetwork = false;
}

ccNetworkManager::~ccNetworkManager()
{
    // TODO Auto-generated destructor stub
    Shutdown();
}

bool ccNetworkManager::Init()
{
    if (_bIsInitNetwork)
        return true;

#if defined(WIN32)
    WORD wVersionRequested = MAKEWORD( 2, 2 );
    WSADATA wsaData;
    int err;

    err = WSAStartup( wVersionRequested, &wsaData );

    if ( err != 0 )
    {
        //// could not find a usable WinSock DLL
        ////cerr << "Could not load winsock" << endl;
        //assert(0); // is this is failing, try a different version that 2.2, 1.0 or later will likely work
        //exit(1);
    }
    else
        _bIsInitNetwork = true;
#endif

    return true;
}


void ccNetworkManager::Shutdown()
{
#if defined(WIN32)
    if (_bIsInitNetwork)
    {
        WSACleanup();

        _bIsInitNetwork = false;
    }
#endif
}

bool ccNetworkManager::GetHostName(std::string& strName)
{
    char szBuffer[1024];

    if (::gethostname(szBuffer, sizeof(szBuffer)) == SOCKET_ERROR)
        return false;

    strName = szBuffer;

    return true;
}


bool ccNetworkManager::GetLocalIP(std::string& strIP)
{
    std::string strHostName;

    if (GetHostName(strHostName) == false)
        return false;

    struct hostent *host = ::gethostbyname((char*)strHostName.c_str());

    if (host == NULL)
        return false;

    //  Obtain the computer's IP
#if defined(WIN32)
    ccString::format(strIP, 
        "%d.%d.%d.%d",
        ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b1,
        ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b2,
        ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b3,
        ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b4);
#else
    strIP = inet_ntoa(*(struct in_addr *)*host->h_addr_list);
#endif

    return true;
}

bool ccNetworkManager::GetNetMaskIP(std::string& strIP)
{

    return false;
}

bool ccNetworkManager::GetGatewayIP(std::string& strIP)
{
    char strHoneName[80];
    
    if (::gethostname(strHoneName, sizeof(strHoneName)) == SOCKET_ERROR)
        return false;

    struct hostent *pHostInfo = ::gethostbyname(strHoneName);

    if (pHostInfo == 0)
    {
        //  std::cout << "Yow! Bad host lookup." << std::endl;
        return false;
    }

    for (int nIndex = 0; pHostInfo->h_addr_list[nIndex] != 0; ++nIndex)
    {
        struct in_addr addr;
        memcpy(&addr, pHostInfo->h_addr_list[nIndex], sizeof(struct in_addr));

        strIP = inet_ntoa(addr);

        break;
        //  std::cout << "Address " << nIndex << ": " << inet_ntoa(addr) << std::endl;
    }

    return true;
}


std::uint32_t ccNetworkManager::convertIP2Int(const std::string& strIP)
{
    std::uint32_t       uConvertIP = 0;
    static const char   digits[] = "0123456789";

    std::uint8_t    tmp[4];
    std::uint8_t    *tp = NULL;

    int saw_digit = 0;
    int octets = 0;
    *(tp = tmp) = 0;

    for (auto item : strIP)
    {
        const char *pch;

        if ((pch = strchr(digits, item)) != NULL)
        {
            std::uint32_t newVal = *tp * 10 + (pch - digits);

            if (newVal > 255)
                return (0);

            *tp = newVal;

            if (!saw_digit)
            {
                if (++octets > 4)
                    return (0);

                saw_digit = 1;
            }
        }
        else
        {
            if (item == '.' && saw_digit)
            {
                if (octets == 4)
                    return (0);

                *++tp = 0;
                saw_digit = 0;
            }
            else
                return (0);
        }
    }

    if (octets < 4)
        return (0);

    uConvertIP = (((std::uint32_t)tmp[0] << 24) & 0xFF000000);
    uConvertIP |= (((std::uint32_t)tmp[1] << 16) & 0x00FF0000);
    uConvertIP |= (((std::uint32_t)tmp[2] << 8) & 0x0000FF00);
    uConvertIP |= (tmp[3] & 0xFF);

    return uConvertIP;
}

bool ccNetworkManager::convertInt2IP(std::uint32_t uIP, std::string& strIP)
{
    ccString::format(strIP, "%d.%d.%d.%d", (uIP >> 24) & 0xFF, (uIP >> 16) & 0xFF, (uIP >> 8) & 0xFF, uIP & 0xFF);

    return true;
}

bool ccNetworkManager::isPublicIP(const std::string& strIP)
{
    char szNum[3];

    if (strIP.length() > 0)
    {
        //  HSPLog("[NetInf] IsPublicIP : %s", strIP.c_str());
    }

    if (strIP.find("10.") == 0 || strIP.find("192.168.") == 0)
        return false;
    else
    {
        if (strIP.find("172.") == 0)
        {
            strncpy(szNum, strIP.c_str() + 4, 3);

            if (szNum[2] != '.')
                return true;

            szNum[2] = 0x00;

            int nValue = atoi(szNum);

            if (nValue >= 16 && nValue <= 31)
                return false;
        }
    }

    return true;
}
