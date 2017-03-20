CFLAGS:= -O3 -g -Wall -Werror -std=c++11
CC:= g++
RPATH:= -Wl,-R../../_build/.lib
LPATH:= -L../../_build/.lib
LIBS:= -lnetwork -lutils -lthread -lroutines -lgames -lsession

all: tttServer tttClient

tttServer: tttServer.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

tttClient: tttClient.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

install:
	mkdir -p ../../_build/06-TTT
	cp tttServer ../../_build/06-TTT
	cp tttClient ../../_build/06-TTT
	cp README ../../_build/06-TTT

clean:
	$(RM) tttServer tttClient

.PHONY: install clean
