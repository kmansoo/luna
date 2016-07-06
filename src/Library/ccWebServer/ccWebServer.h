/*
 * ccWebServer.h
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVER_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVER_H_

#include <vector>
#include <string>
#include <memory>

#include "ccWebServerEventListener.h"

namespace Luna {

class ccWebServer
{
public:
    ccWebServer(const std::string& name, const std::string& ports, const std::string& root_path);
    virtual ~ccWebServer();

public:
    virtual bool Start();
    virtual bool Stop();

public:
    const std::string&  GetName();

    bool    SetOption(std::string const& name, std::string const& value);
    void    SetListener(ccWebServerEventListener* pListener);

protected:
    std::vector<std::string>&    DoGetOptions() const;

protected:
    std::string                 _strName;
    std::string                 _strHttpPorts;
    std::string                 _strWebRootPath;
    ccWebServerEventListener*   _pEventListener;
    std::vector<std::string>    _aOptions;
};

}

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVER_H_ */
