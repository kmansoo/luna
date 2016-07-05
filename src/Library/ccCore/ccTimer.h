/*
 * ccTimer.h
 *
 *  Created on: 2016. 1. 29.
 *      Author: kmansoo
 */

#ifndef LIBRARY_CCCOREAPI_CCTIMER_H_
#define LIBRARY_CCCOREAPI_CCTIMER_H_

#include <functional>
#include <chrono>
#include <future>

class ccTimer
{
public:
    template <class callable, class... arguments>
    ccTimer(int after, bool async, callable&& f, arguments&&... args)
    {
        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

        if (async)
        {
            std::thread([after, task]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(after));
                task();
            }).detach();
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(after));
            task();
        }
    }

};


#endif /* LIBRARY_CCCOREAPI_CCTIMER_H_ */
