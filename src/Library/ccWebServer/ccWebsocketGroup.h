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

namespace Luna {

class ccWebsocketGroup
{
public:
    ccWebsocketGroup(const std::string& strUri);

    virtual ~ccWebsocketGroup();

public:
    const std::string&  GetUri();
    const std::size_t   GetCount() const;

    bool    Add(std::shared_ptr<ccWebsocket> pNewWS);
    bool    Remove(std::shared_ptr<ccWebsocket> pNewWS);
    bool    RemoveAll();

    bool    GetWebsocket(std::int32_t nInstance, std::shared_ptr<ccWebsocket>& pSocket);

    void    Broadcast(const std::string& strMessage);
    void    BroadcastEx(const std::string& strMessage, std::shared_ptr<ccWebsocket>& pExceptSocket);

private:
    std::mutex      _mtx;
    std::string     _strUri;

    std::map< std::int32_t, std::shared_ptr<ccWebsocket> >  _aWSList;
};

}

#endif
