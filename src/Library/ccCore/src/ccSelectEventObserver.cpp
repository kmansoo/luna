/*
* ccThread.cpp
*
*  Created on: 2017. 5. 29.
*      Author: Mansoo Kim(kmansoo@gmail.com)
*/

#include "ccCore/ccSelectEventObserver.h"

#ifndef WIN32
#   include <sys/select.h>
#else
#   include <WinSock2.h>
#endif

namespace Luna {


ccSelectEventObserver::ccSelectEventObserver()  {

    is_stop_thread_ = false;
    polling_thread_ = std::thread(std::bind(&ccSelectEventObserver::poll, this));
}

ccSelectEventObserver::~ccSelectEventObserver() {
    shutdown();
}

void ccSelectEventObserver::shutdown() {
    if (is_stop_thread_ == true)
        return;

    is_stop_thread_ = true;
    polling_thread_.join();
}

bool ccSelectEventObserver::addEvent(int fd, std::function<bool(int eventType, int fd)> f, int mode) {

    auto new_event = std::make_shared<XProbeEventInfo>();

    new_event->handle_ = fd;
    new_event->selectMode_ = mode;
    new_event->callback_func_ = f;

    std::lock_guard<std::mutex> lock(mtx_);

    adding_event_list_.push_back(new_event);

    return true;
}

bool ccSelectEventObserver::removeEvent(int fd) {

    std::lock_guard<std::mutex> lock(mtx_);

    deleting_event_list_.push_back(fd);

    return false;
}

void ccSelectEventObserver::poll() {
    int             maxFd = 0;
    fd_set          read_fds, write_fds, except_fds;
    struct timeval  tv;

    while (is_stop_thread_ == false) {
        manageEvents();

        if (watching_event_list_.size() == 0) {
            std::this_thread::sleep_for(std::chrono::microseconds{ 10 });
            continue;
        }

        tv.tv_sec = 0;
        tv.tv_usec = 50000; //  50ms

        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        FD_ZERO(&except_fds);

        maxFd = -1;

        for (auto it : watching_event_list_) {
            if (it->handle_ < 0)
                continue;
 
            if ((it->selectMode_ & kLunaSelectEventRead) == kLunaSelectEventRead) {
                if (maxFd <= it->handle_)
                    maxFd = it->handle_ + 1;

                FD_SET(it->handle_, &read_fds);
            }

            if ((it->selectMode_ & kLunaSelectEventWrite) == kLunaSelectEventWrite) {
                if (maxFd <= it->handle_)
                    maxFd = it->handle_ + 1;

                FD_SET(it->handle_, &write_fds);
            }

            if ((it->selectMode_ & kLunaSelectEventException) == kLunaSelectEventException) {
                if (maxFd <= it->handle_)
                    maxFd = it->handle_ + 1;

                FD_SET(it->selectMode_, &except_fds);
            }
        }

        if (maxFd == -1) {
            std::this_thread::sleep_for(std::chrono::microseconds{ 10 });
            continue;
        }

        int nResult = select(maxFd, &read_fds, &write_fds, &except_fds, &tv);

        if (nResult > 0)
        {
            manageEvents();

            for (auto it : watching_event_list_) {
                if (FD_ISSET(it->handle_, &read_fds)) {
                    if (it->callback_func_) {
                        it->callback_func_(kLunaSelectEventRead, it->handle_);
                        break;
                    }
                }

                if (FD_ISSET(it->handle_, &write_fds)) {
                    if (it->callback_func_) {
                        it->callback_func_(kLunaSelectEventWrite, it->handle_);
                        break;
                    }
                }

                if (FD_ISSET(it->handle_, &except_fds)) {
                    if (it->callback_func_) {
                        it->callback_func_(kLunaSelectEventException, it->handle_);
                        break;
                    }
                }
            }
        }
    }
}

void ccSelectEventObserver::manageEvents()
{
    mtx_.lock();

    for (auto it : adding_event_list_)
        watching_event_list_.push_back(it);

    for (auto fd : deleting_event_list_) {
        for (auto it : watching_event_list_) {
            if (it->handle_ == fd) {
                watching_event_list_.remove(it);
                break;
            }
        }
    }

    adding_event_list_.clear();
    deleting_event_list_.clear();

    mtx_.unlock();
}

}
