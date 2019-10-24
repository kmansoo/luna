/*
 * ccCoreAPI.h
 *
 *  Created on: 2016. 1. 26.
 *      Author: Mansoo Kim(kmansoo@gmail.com)
 */

#pragma once

namespace Luna {

void    sleep(int milliseconds);
void    usleep(int microseconds);

unsigned long long get_tick_count();     //  tick count(ms)

}
