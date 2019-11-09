CXX = g++

PREFIX ?= /usr/local
HEADER = EventEmitter.hpp
DEMO = emitter

demo : $(TARGET)
	$(CXX) -std=c++11 -I$(PREFIX)/include -fPIC demo.cpp -lpthread -o $(DEMO)
    
install :
	install -m 644 $(HEADER) $(PREFIX)/include

uninstall :
	rm -f $(PREFIX)/include/$(HEADER)

clean :
	-rm -f $(DEMO)
