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

class emitter : public EventEmitter {
};

int main(int argc, const char * argv[]) {
    emitter emitter;
    emitter.on("event", [&emitter](int data) {
        ostringstream osstream;
        osstream << "data: " << data << '\n';
        std::cout<<osstream.str();
    });
    
    vector<thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back([&emitter, i]() {
            emitter.emit("event", i);
        });
    }
    
    for (auto &t : threads) t.join();
}
