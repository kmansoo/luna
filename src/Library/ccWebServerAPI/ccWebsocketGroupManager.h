/*
* ccWebsocketGroupManager.h
*
*  Created on: 2015. 12. 14.
*      Author: kmansoo
*/

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSOCKETGROUP_MANAGER_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSOCKETGROUP_MANAGER_H_

#include <string>
#include <map>
#include <memory>
#include <mutex>

#include "ccWebsocketGroup.h"

class ccWebsocketGroupManager
{
public:
    ccWebsocketGroupManager();
    virtual ~ccWebsocketGroupManager();

public:
    bool    Add(std::shared_ptr<ccWebsocket> pNewWS);
    bool    Remove(std::shared_ptr<ccWebsocket> pNewWS);
    bool    RemoveAll();

private:
    std::mutex      _mtx;

    std::map<std::string, std::shared_ptr<ccWebsocketGroup>>  _aWSGList;
};

#endif
