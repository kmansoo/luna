/*
 * ccSingletonT.h
 *
 *  Created on: 2016. 1. 26.
 *      Author: kmansoo
 */

#ifndef LIBRARY_CCCOREAPI_CCSINGLETONT_H_
#define LIBRARY_CCCOREAPI_CCSINGLETONT_H_

namespace Luna {

    template<typename T>
    class ccSingletonT
    {
    public:
        static T& getInstance()
        {
            static T me;
            return me;
        }
    };

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
