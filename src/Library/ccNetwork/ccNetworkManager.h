/*
 * ccNetworkManager.h
 *
 *  Created on: 2016. 1. 27.
 *      Author: kmansoo
 */

#ifndef LIBRARY_CCNETWORKAPI_CCNETWORKMANAGER_H_
#define LIBRARY_CCNETWORKAPI_CCNETWORKMANAGER_H_

#include <string>

#include "ccCore/ccSingletonT.h"

class ccNetworkManager : public ccSingletonT<ccNetworkManager>
{
public:
    virtual ~ccNetworkManager();

protected:
    friend class ccSingletonT<ccNetworkManager>;
    ccNetworkManager();

public:
    bool    Init();
    void    Shutdown();

    bool    GetLocalIP(std::string& strIP);
    bool    GetNetMaskIP(std::string& strIP);
    bool    GetGatewayIP(std::string& strIP);

    std::uint32_t   convertIP2Int(const std::string& strIP);
    bool            convertInt2IP(std::uint32_t uIP, std::string& strIP);
    bool            isPublicIP(const std::string& strIP);

    bool            GetHostName(std::string& strName);

private:
    bool    _bIsInitNetwork;
};

#endif /* LIBRARY_CCNETWORKAPI_CCNETWORKMANAGER_H_ */
