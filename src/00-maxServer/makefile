CFLAGS:= -O3 -g -Wall -Werror -std=c++11
CC:= g++
RPATH:= -Wl,-R../../_build/.lib
LPATH:= -L../../_build/.lib
LIBS:= -lnetwork

all: maxServer maxClient

maxServer: maxServer.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

maxClient: maxClient.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

install:
	mkdir -p ../../_build/00-maxServer
	cp maxServer ../../_build/00-maxServer
	cp maxClient ../../_build/00-maxServer
	cp README ../../_build/00-maxServer

clean:
	$(RM) maxServer maxClient

.PHONY: install clean
