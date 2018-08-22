/*
* ccSelectEventObserver.h
*
*  Created on: 2017.5.26
*      Author: Mansoo Kim(kmansoo@gmail.com)
*/

#pragma once

#include <thread>
#include <list>
#include <mutex>

#include "ccSingleton.h"

namespace Luna {

class ccSelectEventObserver : public ccSingleton<ccSelectEventObserver>
{
public:
    ccSelectEventObserver();
    virtual ~ccSelectEventObserver();

public:
    enum LunaSelectEventType {
        kLunaSelectEventRead = 0x01,
        kLunaSelectEventWrite = 0x02,
        kLunaSelectEventException = 0x04,

        kLunaSelectEventAll = 0x07,
    };

public:
    void  shutdown();

    bool  add_event(int fd, std::function<bool(int eventType, int fd)> f, int mode = kLunaSelectEventRead);
    bool  remove_event(int fd);

private:
    void poll();
    void manage_events();

protected:
    class XProbeEventInfo
    {
    public:
        XProbeEventInfo() {
            select_mode_ = 0x00;
            handle_ = -1;
        }

    public:
        int select_mode_;
        int handle_;
        std::function<bool(int eventType, int fd)> callback_func_;
    };

private:
    std::thread polling_thread_;
    bool is_stop_thread_;

    std::mutex mtx_;
    
    std::list<std::shared_ptr<XProbeEventInfo>> watching_event_list_;
    std::list<std::shared_ptr<XProbeEventInfo>> adding_event_list_;
    std::list<int> deleting_event_list_;
};

}
