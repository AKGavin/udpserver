INCLUDE=-I/usr/include/mysql/ -I/usr/include/
#LIBS=-L/usr/lib/mysql/lib 
LIBS= -l/usr/lib/libuuid.a  -l/usr/lib/i386-linux-gnu/libmysqlclient.a
CFLAGS = -g -Wall -fPIC
TARGET=../lib/libframecommon.a
		
BINARY = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all:$(TARGET)
	@cp *.h ../include/framecommon

%.o:%.cpp
	$(CXX) $< -o $@ -c $(INCLUDE) $(CFLAGS) $(LIBS) 

clean:
	@rm -rf ${BINARY} ${TARGET}
	@rm -f ../include/framecommon/*

$(TARGET):$(BINARY)
	ar rcs $@ $^
