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
    const std::string&  getUri();
    const std::size_t   getCount() const;

    bool    add(std::shared_ptr<ccWebsocket> pNewWS);
    bool    remove(std::shared_ptr<ccWebsocket> pNewWS);
    bool    removeAll();

    bool    getWebsocket(std::int32_t nInstance, std::shared_ptr<ccWebsocket>& pSocket);

    void    broadcast(const std::string& strMessage);
    void    broadcastEx(const std::string& strMessage, std::shared_ptr<ccWebsocket>& pExceptSocket);

private:
    std::mutex      _mtx;
    std::string     _strUri;

    std::map< std::int32_t, std::shared_ptr<ccWebsocket> >  _aWSList;
};

}

#endif
