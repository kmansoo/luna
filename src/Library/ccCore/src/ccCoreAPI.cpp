/*
 * ccCoreAPI.cpp
 *
 *  Created on: 2016. 1. 26.
 *      Author: Mansoo Kim(kmansoo@gmail.com)
 */

#include <thread>
#include <chrono>

#include "ccCore/ccCoreAPI.h"

namespace Luna {

//
//  I'll use following code!
//
//// Clock built upon Windows GetTickCount()
//struct TickCountClock
//{
//    typedef unsigned long long                       rep;
//    typedef std::milli                               period;
//    typedef std::chrono::duration<rep, period>       duration;
//    typedef std::chrono::time_point<TickCountClock>  time_point;
//    static const bool is_steady = true;
//
//    static time_point now() noexcept
//    {
//        return time_point(duration(GetChronoTickCount()));
//    }
//};


void    sleep(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds{ milliseconds });
}

void    usleep(int microseconds) {
    std::this_thread::sleep_for(std::chrono::microseconds{ microseconds });
}

unsigned long long get_tick_count() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

}

