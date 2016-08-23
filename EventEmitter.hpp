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
#include <string>
#include <Functor.hpp>

class EventEmitter {
public:
    /**
     *  @brief Deconstructor
     */
    ~EventEmitter() {
        std::for_each(events.begin(), events.end(), [](std::pair<std::string, Functor *> pair) {
            delete pair.second;
        });
        events.clear();
    }
    
    /**
     *  @brief Event setter
     *
     *  @param event  Event name
     *  @param lambda Callback function when event emitted
     */
    template <typename Function>
    void on(const std::string& event, Function&& lambda) {
        events[event] = new Functor{std::forward<Function>(lambda)};
    }
    
    /**
     *  @brief Event emitter
     *
     *  @param event  Event name
     */
    template <typename ... Arg>
    void emit(const std::string& event, Arg&& ... args) {
        Functor * on = events[event];
        if (on) (*on)(std::forward<Arg>(args)...);
    }
    
    /**
     *  @brief Event name - Callback function
     */
    std::map<std::string, Functor *> events;

protected:
    /**
     *  @brief Constructor
     */
    EventEmitter() {
    };
};

#endif /* EVENTEMITTER_HPP */
