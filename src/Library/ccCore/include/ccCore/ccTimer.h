/*
 * ccTimer.h
 *
 *  Created on: 2016. 1. 29.
 *      Author: Mansoo Kim(kmansoo@gmail.com)
 */

#pragma once

#include <functional>
#include <chrono>
#include <future>

namespace Luna {

class ccTimer {
public:
    template <class callable, class... arguments>
    ccTimer(int after, bool async, callable&& f, arguments&&... args) {
        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

        if (async) {
            std::thread([after, task]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(after));
                task();
            }).detach();
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(after));
            task();
        }
    }
};

};
