/*
 * ccCoreAPI.h
 *
 *  Created on: 2016. 1. 26.
 *      Author: kmansoo
 */

#ifndef LIBRARY_CCCOREAPI_CCCOREAPI_H_
#define LIBRARY_CCCOREAPI_CCCOREAPI_H_

namespace Luna {

void    sleep(int milliseconds);
void    usleep(int milliseconds);

unsigned long long getTickCount();     //  tick count(ms)

}

#endif /* LIBRARY_CCCOREAPI_CCCOREAPI_H_ */
