#pragma once

#include <functional>

class Subscriber {
    std::function<void(bool)> m_callback;
public:
    Subscriber() : m_callback(nullptr) {}
    Subscriber(std::function<void(bool)> cb) : m_callback(cb) {}
    Subscriber(const Subscriber& s) {m_callback = s.m_callback;}

    virtual void operator()(bool val){
        return m_callback(val);
    }

    virtual Subscriber& operator=(const Subscriber& s) {m_callback = s.m_callback; return *this;}
    virtual Subscriber& operator=(std::function<void(bool)> cb) {m_callback = cb; return *this;}
};