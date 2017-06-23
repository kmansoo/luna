#ifndef EASYWSCLIENT_HPP_20120819_MIOFVASDTNUASZDQPLFD
#define EASYWSCLIENT_HPP_20120819_MIOFVASDTNUASZDQPLFD

// This code comes from:
// https://github.com/dhbaird/easywsclient
//
// To get the latest version:
// wget https://raw.github.com/dhbaird/easywsclient/master/easywsclient.hpp
// wget https://raw.github.com/dhbaird/easywsclient/master/easywsclient.cpp

#include <string>
#include <vector>

namespace easywsclient {

#if __cplusplus > 199711L || defined(_WINDOWS)   //  supports std++ 11
    struct Callback_Imp { virtual void operator()(const std::string& message) = 0; };
    struct BytesCallback_Imp { virtual void operator()(const std::vector<uint8_t>& message) = 0; };
#else
class DispatchFunction
{
private:
    typedef void(*function_type1)(const std::string& message);
    typedef void(*function_type2)(const std::vector<uint8_t>& message);

    // Abstract interface for all callbacks
    class FunctionCaller
    {
    public:
        virtual ~FunctionCaller() { }

    public:
        virtual FunctionCaller* clone() const = 0;

        virtual void call(const std::string& message) = 0;
        virtual void call(const std::vector<uint8_t>& message) = 0;
    };

    // Implementation for member functions
    template <typename T>
    class ClassMethodCaller : public FunctionCaller
    {
    public:
        typedef void(T::*method_type1)(const std::string& message);
        typedef void(T::*method_type2)(const std::vector<uint8_t>& message);

    private:
        T*              object_;
        method_type1    callback_method1_;
        method_type2    callback_method2_;

    public:
        ClassMethodCaller(T* obj, method_type1 user_method) : object_(obj), callback_method1_(user_method) {}
        ClassMethodCaller(T* obj, method_type2 user_method) : object_(obj), callback_method2_(user_method) {}
        ClassMethodCaller(T* obj, method_type1 user_method1, method_type2 user_method2) : object_(obj), callback_method1_(user_method1), callback_method2_(user_method2) {}

        virtual FunctionCaller* clone() const 
        { 
            return new ClassMethodCaller<T>(object_, callback_method1_, callback_method2_);
        }

        virtual void call(const std::string& message)
        {
            (object_->*callback_method1_)(message);
        }

        virtual void call(const std::vector<uint8_t>& message)
        {
            (object_->*callback_method2_)(message);
        }
    };

    // Implementation for global functions
    class GlobalFunctionCaller : public FunctionCaller
    {
    private:
        function_type1 api_1_;
        function_type2 api_2_;

    public:
        GlobalFunctionCaller(function_type1 user_func) : api_1_(user_func) {}
        GlobalFunctionCaller(function_type2 user_func) : api_2_(user_func) {}
        GlobalFunctionCaller(function_type1 user_func1, function_type2 user_func2) : api_1_(user_func1), api_2_(user_func2) {}

        virtual FunctionCaller* clone() const { return new GlobalFunctionCaller(api_1_, api_2_); }

        virtual void call(const std::string& message)
        {
            (*api_1_)(message);
        }

        virtual void call(const std::vector<uint8_t>& message)
        {
            (*api_2_)(message);
        }
    };

private:
    // Data member for the Function class
    FunctionCaller*       callers_[2];

public:
    // basic construct/destruct
    DispatchFunction() {
        for (int nIndex = 0; nIndex < 2; nIndex++)
            callers_[nIndex] = 0;
    }

    ~DispatchFunction()
    {
        for (int nIndex = 0; nIndex < 2; nIndex++)
            delete callers_[nIndex];
    }

    // copy ctor/assign
    DispatchFunction(const DispatchFunction& rhs)
    {
        for (int nIndex = 0; nIndex < 2; nIndex++)
        {
            callers_[nIndex] = 0;

            if (rhs.callers_[nIndex])    callers_[nIndex] = rhs.callers_[nIndex]->clone();
        }
    }

    DispatchFunction& operator = (const DispatchFunction& rhs)
    {
        for (int nIndex = 0; nIndex < 2; nIndex++)
        {
            if (callers_[nIndex])
            {
                delete callers_[nIndex];
                callers_[nIndex] = 0;
            }

            if (rhs.callers_[nIndex])
                callers_[nIndex] = rhs.callers_[0]->clone();
        }

        return *this;
    }

    // construct with an actual callback
    DispatchFunction(function_type1 user_func)
    {
        callers_[0] = new GlobalFunctionCaller(user_func);
        callers_[1] = 0;
    }

    DispatchFunction(function_type2 user_func)
    {
        callers_[0] = 0;
        callers_[1] = new GlobalFunctionCaller(user_func);
    }

    template <typename T>
    //	HSWebsocketFunction(T* obj, ClassMethodCaller<T>::webapi_method user_method )
    DispatchFunction(void(T::*user_method)(const std::string& message), T* obj)
    {
        callers_[0] = new ClassMethodCaller<T>(obj, user_method);
        callers_[1] = 0;
    }

    template <typename T>
    //	HSWebsocketFunction(T* obj, ClassMethodCaller<T>::webapi_method user_method )
    DispatchFunction(void(T::*user_method)(const std::vector<uint8_t>& message), T* obj)
    {
        callers_[0] = 0;
        callers_[1] = new ClassMethodCaller<T>(obj, user_method);
    }

    // actually calling the function
    void operator () (const std::string& message)
    {
        if (callers_[0])
            callers_[0]->call(message);
    }

    void operator () (const std::vector<uint8_t>& message)
    {
        if (callers_[1])
            callers_[1]->call(message);
    }

    bool    HasMethod(int nType)
    {
        if (nType >= 0 && nType <= 2)
        {
            if (callers_[nType])
                return true;
        }

        return false;
    }
};

#endif

class WebSocket {
  public:
    typedef WebSocket * pointer;
    typedef enum readyStateValues { CLOSING, CLOSED, CONNECTING, OPEN } readyStateValues;

    // Factories:
    static pointer create_dummy();
    static pointer from_url(const std::string& url, const std::string& origin = std::string());
    static pointer from_url_no_mask(const std::string& url, const std::string& origin = std::string());

    // Interfaces:
    virtual ~WebSocket() { }
    virtual void poll(int timeout = 0) = 0; // timeout in milliseconds
    virtual void send(const std::string& message) = 0;
    virtual void send_binary(const std::string& message) = 0;
    virtual void send_binary(const std::vector<uint8_t>& message) = 0;
    virtual void sendPing() = 0;
    virtual void close() = 0;
    virtual readyStateValues getReadyState() const = 0;
    virtual std::int32_t    getInstance() const = 0;

#if __cplusplus > 199711L || defined(_WINDOWS)
    template<class Callable>
    void dispatch(Callable callable)
        // For callbacks that accept a string argument.
    { // N.B. this is compatible with both C++11 lambdas, functors and C function pointers
        struct _Callback : public Callback_Imp {
            Callable& callable;
            _Callback(Callable& callable) : callable(callable) { }
            void operator()(const std::string& message) { callable(message); }
        };
        _Callback callback(callable);
        _dispatch(callback);
	}

    template<class Callable>
    void dispatchBinary(Callable callable)
        // For callbacks that accept a std::vector<uint8_t> argument.
    { // N.B. this is compatible with both C++11 lambdas, functors and C function pointers
        struct _Callback : public BytesCallback_Imp {
            Callable& callable;
            _Callback(Callable& callable) : callable(callable) { }
            void operator()(const std::vector<uint8_t>& message) { callable(message); }
        };
        _Callback callback(callable);
        _dispatchBinary(callback);
    }
#else
    void dispatch(easywsclient::DispatchFunction f)
    {
        _dispatch(f);
    }

    void dispatchBinary(easywsclient::DispatchFunction f)
    {
        _dispatchBinary(f);
    }
#endif

  protected:

#if __cplusplus > 199711L || defined(_WINDOWS)
      virtual void _dispatch(Callback_Imp& callable) = 0;
      virtual void _dispatchBinary(BytesCallback_Imp& callable) = 0;
#else
    virtual void _dispatch(easywsclient::DispatchFunction f) = 0;
    virtual void _dispatchBinary(easywsclient::DispatchFunction f) = 0;
#endif
};

} // namespace easywsclient

#endif /* EASYWSCLIENT_HPP_20120819_MIOFVASDTNUASZDQPLFD */
