/*
 * ccThread.h
 *
 *  Created on: 2017. 5. 29.
 *      Author: Mansoo Kim(kmansoo@gmail.com)
 */

#ifndef LIBRARY_CCCOREAPI_CCTHREAD_H_
#define LIBRARY_CCCOREAPI_CCTHREAD_H_

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
    ccThread(bool release, callable&& f, arguments&&... args) {

    }

private:
    std::thread thread_;
    bool is_stop_thread_;

};

};

#endif /* LIBRARY_CCCOREAPI_CCTHREAD_H_ */