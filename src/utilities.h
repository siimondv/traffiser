//
// Created by root on 3/9/24.
//

#include <exception>
#include <string>


#ifndef NETWORK_SNIFFER_UTILITIES_H
#define NETWORK_SNIFFER_UTILITIES_H

using namespace std;

class common_exception : public exception
{
public:
    common_exception(const common_exception& exp): _msg (exp._msg)
    {
    }

    common_exception(const string& msg): _msg (msg)
    {
    }

    common_exception(const char* msg): _msg (msg)
    {
    }

    virtual const char * what () const throw ()
    {
        return _msg.c_str();
    }
    ~common_exception() throw()
    {}
private:
    string _msg;
};

template <class E> void check (bool arg1, const E& e) noexcept(false)
{
    if (!arg1)
        throw E(e);
}


#endif //NETWORK_SNIFFER_UTILITIES_H
