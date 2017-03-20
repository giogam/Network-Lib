CFLAGS:= -O3 -g -Wall -Werror -std=c++11
CC:= g++
RPATH:= -Wl,-R../../_build/.lib
LPATH:= -L../../_build/.lib
LIBS:= -lnetwork  -lutils

all: calcServer calcClient

calcServer: calcServer.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

calcClient: calcClient.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

install:
	mkdir -p ../../_build/02-calc
	cp calcServer ../../_build/02-calc
	cp calcClient ../../_build/02-calc
	cp README ../../_build/02-calc

clean:
	$(RM) calcServer calcClient

.PHONY: install clean
