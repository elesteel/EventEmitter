//
//  EventEmitter.hpp
//  EventEmitter
//
//  Created by BlueCocoa on 2016/8/6.
//  Copyright © 2016 BlueCocoa. All rights reserved.
//

#ifndef EVENTEMITTER_HPP
#define EVENTEMITTER_HPP

#include <map>
#include <mutex>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>
#include <set>
#include <algorithm> //std::forEach()
#include "Functor.h"

class EventEmitter {
public:
    using handler_type = Functor*;
    /**
     *  @brief Deconstructor
     */
    ~EventEmitter() {
        for(auto it = registry_.begin(); it != registry_.end(); ++it) {
            delete it->first;
        }
    }
    
    /**
     *  @brief Event setter
     *
     *  @param event  Event name
     *  @param lambda Callback function when event emitted
     *  @return a handle which can be used in removeListener
     */
    template <typename Function>
    handler_type on(const std::string& event, Function&& lambda) {
        std::unique_lock<std::mutex> locker(_events_mtx);
        auto func = new Functor{std::forward<Function>(lambda)};
        // events[event].emplace_back(func, false);
        // events[event][func] = std::make_tuple(func, false);
        // listeners_[func] = event;
        listeners_[event].insert(func);
        registry_[func] = event;
        return func;
    }
    
    /**
     *  @brief Once event
     *
     *  @param event  Event name
     *  @param lambda Callback function when event emitted
     *  @return a handle which can be used in removeListener
     */
    template <typename Function>
    handler_type once(const std::string& event, Function&& lambda) {
        std::unique_lock<std::mutex> locker(_events_mtx);
        auto func = new Functor{std::forward<Function>(lambda)};
        // events[event].emplace_back(new Functor{std::forward<Function>(lambda)}, true);
        // events[event][func] = std::make_tuple(func, true);
        // listeners_[func] = event;
        once_listeners_[event].insert(func);
        registry_[func] = event;
        return func;
    }
    
    /**
     *  @brief Event emitter
     *
     *  @param event  Event name
     */
    template <typename ... Arg>
    void emit(const std::string& event, Arg&& ... args) {
        std::unique_lock<std::mutex> locker(_events_mtx);

        if (listeners_.find(event) != listeners_.end()) {
            auto& listeners = listeners_[event];
            for(auto it = listeners.begin(); it != listeners.end(); ++it) {
                Functor *func = *it;
                if (func) {
                    (*func) (std::forward<Arg>(args)...);
                }
            }
        }

        if (once_listeners_.find(event) != once_listeners_.end()) {
            auto& once_listeners = once_listeners_[event];
            for(auto it = once_listeners.begin(); it != once_listeners.end(); ++it) {
                Functor *func = *it;
                if (func) {
                    (*func) (std::forward<Arg>(args)...);
                    registry_.erase(func);
                    delete func;
                }
            }
            once_listeners.clear();
            once_listeners_.erase(event);
        }        
    }
    
    /**
     *  @brief Number of listeners
     *
     *  @param event  Event name
     */
    ssize_t listener_count(const std::string& event) {
        std::unique_lock<std::mutex> locker(_events_mtx);
        ssize_t count = 0;
        auto it = listeners_.find(event);
        if (it != listeners_.end()) {
            count += listeners_[event].size();
        }
        auto it2 = once_listeners_.find(event);
        if (it2 != once_listeners_.end()) {
            count += once_listeners_[event].size();
        }
        return count;
    }

    void removeListener(handler_type listener) {
        std::unique_lock<std::mutex> locker(_events_mtx);
        if (registry_.find(listener) == registry_.end()) {
            return;
        }
        auto event = registry_[listener];
        if (listeners_.find(event) != listeners_.end()) {
            listeners_[event].erase(listener);
            if (listeners_[event].empty()) {
                listeners_.erase(event);
            }
        }

        if (once_listeners_.find(event) != once_listeners_.end()) {
            once_listeners_[event].erase(listener);
            if (once_listeners_[event].empty()) {
                once_listeners_.erase(event);
            }
        }
    }
    
protected:
    /**
     *  @brief Constructor
     */
    EventEmitter() {
    };
    

    std::map<std::string, std::set<Functor*>> listeners_;

    std::map<std::string, std::set<Functor*>> once_listeners_;

    std::map<Functor*, std::string> registry_;
    
private:
    /**
     *  @brief Mutex for events
     */
    std::mutex _events_mtx;
};


class Binder final {
public:
    Binder(EventEmitter& target)
    : target_(target) {}

    ~Binder() 
    {
        for(auto handle : registry_) {
            target_.removeListener(handle);
        }
    }

    template <typename ... Arg>
    void on(Arg&& ... args) {
        auto handle = target_.on(std::forward<Arg>(args)...);
        registry_.push_back(handle);
    }

private:
    EventEmitter& target_;
    std::vector<EventEmitter::handler_type> registry_;
};

#endif /* EVENTEMITTER_HPP */
