//
//  main.cpp
//  EventEmitter
//
//  Created by BlueCocoa on 2016/8/23.
//  Copyright © 2016 BlueCocoa. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include "EventEmitter.hpp"

using namespace std;

class emitter : public EventEmitter {};
using Emitter = emitter;

int main(int argc, const char * argv[]) {
    emitter emitter;
    auto listener1 = emitter.on("event", [&emitter](int data) {
        ostringstream osstream;
        osstream << "[Listener 1] data: " << data << '\n';
        std::cout<<osstream.str();
    });
    emitter.on("event", [&emitter](int data) {
        ostringstream osstream;
        osstream << "[Listener 2] data: " << data << '\n';
        std::cout<<osstream.str();
    });
    emitter.once("event", [&emitter](int data) {
        ostringstream osstream;
        osstream << "[Once Listener] data: " << data << '\n';
        std::cout<<osstream.str();
    });
    emitter.once("event", [&emitter](int data) {
        ostringstream osstream;
        osstream << "[Once Listener 2] data: " << data << '\n';
        std::cout<<osstream.str();
    });
    

    {
        Emitter e2;
        e2.on("something", [](){
            std::cout << "something happen" << std::endl;
        });
        auto init = e2.once("init", [](){
            std::cout << "init complete" << std::endl;
        });
        e2.removeListener(init);
        e2.emit("init");
        e2.emit("something", "hello");

        Binder binder(emitter);
        binder.on("event", [](){
            std::cout << "event handle in binder" << std::endl;
        });

        std::cout<<"in block" << emitter.listener_count("event")<<" listeners for 'event'\n";
        emitter.emit("event", 111);
    }

    std::cout<<emitter.listener_count("event")<<" listeners for 'event'\n";

    emitter.emit("event", 100);

    emitter.removeListener(listener1);

    emitter.emit("event", 200);
    
    vector<thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back([&emitter, i]() {
            emitter.emit("event", i);
        });
    }
    
    for (auto &t : threads) t.join();
    
    std::cout<<emitter.listener_count("event")<<" listeners for 'event'\n";

    
}
