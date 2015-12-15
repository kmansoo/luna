/*
* ccWebsocketGroup.h
*
*  Created on: 2015. 12. 14.
*      Author: kmansoo
*/

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSOCKETGROUP_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSOCKETGROUP_H_

#include <string>
#include <map>
#include <memory>
#include <mutex>

#include "ccWebsocket.h"

class ccWebsocketGroup
{
public:
    ccWebsocketGroup(const std::string& strUri);
    ccWebsocketGroup(const char* pUri, std::size_t size);

    virtual ~ccWebsocketGroup();

public:
    const std::string&  GetUri();
    const std::size_t   GetCount() const;

    bool    Add(std::shared_ptr<ccWebsocket> pNewWS);
    bool    Remove(std::shared_ptr<ccWebsocket> pNewWS);
    bool    RemoveAll();

    bool    GetWebsocket(std::int32_t nInstance, std::shared_ptr<ccWebsocket>& pSocket);

    void    Broadcast(const char* strMessage, std::size_t size);
    void    Broadcast(const std::string strMessage);

private:
    std::mutex      _mtx;
    std::string     _strUri;

    std::map< std::int32_t, std::shared_ptr<ccWebsocket> >  _aWSList;
};

#endif
