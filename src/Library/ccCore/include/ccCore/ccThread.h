/*
 * ccThread.h
 *
 *  Created on: 2017. 5. 29.
 *      Author: Mansoo Kim(kmansoo@gmail.com)
 */

#pragma once

#include <functional>
#include <chrono>
#include <future>

namespace Luna {

//  Under construction
//
//  2017.6.4, I'll implement this class after designing functions and usage.
class ccThread {
public:
    template <class callable, class... arguments>
    ccThread(bool release, callable&& f, arguments&&... args) : is_stop_thread_(false) {

    }

    bool set_stop() {
        is_stop_thread_ = true;

        return true;
    }

private:
    std::thread thread_;
    bool is_stop_thread_;

};

};
