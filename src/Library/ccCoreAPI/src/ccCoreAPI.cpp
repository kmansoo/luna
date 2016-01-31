/*
 * ccCoreAPI.cpp
 *
 *  Created on: 2016. 1. 26.
 *      Author: kmansoo
 */

#include <thread>

#include "ccCoreAPI.h"

namespace Luna {

void    sleep(int milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds{ milliseconds });
}

void    usleep(int microseconds)
{
    std::this_thread::sleep_for(std::chrono::microseconds{ microseconds });
}

}

