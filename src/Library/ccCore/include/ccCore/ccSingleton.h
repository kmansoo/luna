/*
 * ccSingletonT.h
 *
 *  Created on: 2016. 1. 26.
 *      Author: Mansoo Kim(kmansoo@gmail.com)
 */

#ifndef LIBRARY_CCCOREAPI_CCSINGLETON_H_
#define LIBRARY_CCCOREAPI_CCSINGLETON_H_

#include <mutex>
#include <memory>

namespace Luna {

template<typename T>
class ccSingleton {
public:
    static T& instance() {
        std::call_once(singleton_flag_, []() {
            instance_ = std::make_shared<T>();
        });

        return *instance_;
    }

private:
    static std::once_flag singleton_flag_;
    static std::shared_ptr<T> instance_;
};

template<typename T> 
std::once_flag      ccSingleton<T>::singleton_flag_;

template<typename T> 
std::shared_ptr<T>  ccSingleton<T>::instance_;

//
//  example

/*
    class foo : public ccSingletonT<foo>
    {
    public:
        ~foo()
        {
            _nCount = 0;
        }

    protected:
        foo() : _nCount(0) {}

        //  If necessary
        friend class ccSingletonT<foo>;

    public:
        void show() {
            std::cout << " count = " << _nCount++ << std::endl;
        }

    public:
        int _nCount;
    };

    int main()
    {
        while (foo::getInstance()._nCount < 100)
            foo::getInstance().show();

        return 0;
    }

*/
}

#endif /* LIBRARY_CCCOREAPI_CCSINGLETONT_H_ */
