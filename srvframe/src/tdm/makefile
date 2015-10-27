include ../makefile.comm
BINARY = $(patsubst %.cpp,$(SERVER_ROOT)/obj/%.o,$(wildcard *.cpp))

all:$(BINARY)
	cd common;make;cd ..
	cd parser;make;cd ..
	cd stats;make;cd ..
	cd thread;make;cd ..
	cd whitelist;make;cd ..

$(SERVER_ROOT)/obj/%.o:%.cpp
	$(CXX) $< -o $@ -c $(INCLUDE) $(CFLAGS)

clean:
	@rm -rf ${BINARY} 
	cd common;make clean;cd ..
	cd parser;make clean;cd ..
	cd stats;make clean;cd ..
	cd thread;make clean;cd ..
	cd whitelist;make clean;cd ..

install:
	@cp *.h ${SERVER_ROOT}/include
