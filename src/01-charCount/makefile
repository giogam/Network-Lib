CFLAGS:= -O3 -g -Wall -Werror -std=c++11
CC:= g++
RPATH:= -Wl,-R../../_build/.lib
LPATH:= -L../../_build/.lib
LIBS:= -lnetwork  -lutils

all: countServer countClient

countServer: countServer.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

countClient: countClient.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

install:
	mkdir -p ../../_build/01-charCount
	cp countServer ../../_build/01-charCount
	cp countClient ../../_build/01-charCount
	cp README ../../_build/01-charCount

clean:
	$(RM) countServer countClient

.PHONY: install clean
