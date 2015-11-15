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

using namespace std;

class ccWebServer
{
public:
    ccWebServer(const char* name, const char* ports);
    virtual ~ccWebServer();

public:
    virtual bool Start();
    virtual bool Stop();

public:
    const string&  GetName();

    bool    SetOption(string const& name, string const& value);
    void    SetListener(ccWebServerEventListener* pListener);

protected:
    vector<string>&    DoGetOptions() const;

protected:
    string                      _strName;
    string                      _strHttpPorts;
    ccWebServerEventListener*   _pEventListener;
    vector<string>              _aOptions;
};


#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVER_H_ */
