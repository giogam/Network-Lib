CFLAGS:= -O3 -g -Wall -Werror -std=c++11
CC:= g++
RPATH:= -Wl,-R../../_build/.lib
LPATH:= -L../../_build/.lib
LIBS:= -lnetwork -lutils -lthread -lroutines -lgames -lsession

all: ttt-ftServer ttt-ftClient

ttt-ftServer: ttt-ftServer.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

ttt-ftClient: ttt-ftClient.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

install:
	mkdir -p ../../_build/08-TTT-FT
	mkdir -p ../../_build/08-TTT-FT/resources
	mkdir -p ../../_build/08-TTT-FT/resources/matches
	cp ttt-ftServer ../../_build/08-TTT-FT
	cp ttt-ftClient ../../_build/08-TTT-FT
	cp README ../../_build/08-TTT-FT

clean:
	$(RM) ttt-ftServer ttt-ftClient

.PHONY: install clean
