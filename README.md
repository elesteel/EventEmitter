# EventEmitter
A simple EventEmitter in C++

Tested on macOS

### Dependency
[Functor](https://github.com/BlueCocoa/Functor)

### Installation
```$ sudo make install```

### Usage

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

### Screenshots

![Screenshots](https://raw.githubusercontent.com/BlueCocoa/EventEmitter/master/screenshot.png)
