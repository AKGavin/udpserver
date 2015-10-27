include ../makefile.comm
BINARY = $(patsubst %.cpp,$(SERVER_ROOT)/obj/%.o,$(wildcard *.cpp))

all:$(BINARY)

$(SERVER_ROOT)/obj/%.o:%.cpp
	$(CXX) $< -o $@ -c $(INCLUDE) $(CFLAGS)

install:
	@cp *.h ${SERVER_ROOT}/include

clean:
	@rm -rf ${BINARY} 
