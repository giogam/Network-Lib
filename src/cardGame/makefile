CFLAGS:= -O3 -g -Wall -Werror -std=c++11
CC:= g++
RPATH:= -Wl,-R../../_build/.lib
LPATH:= -L../../_build/.lib
LIBS:= -lnetwork -lutils -lthread -lroutines -lgames -lsession

all: gameServer gameClient

gameServer: gameServer.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

gameClient: gameClient.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

install:
	mkdir -p ../../_build/cardGame
	mkdir -p ../../_build/cardGame/resources
	mkdir -p ../../_build/cardGame/resources/matches
	cp gameServer ../../_build/cardGame
	cp gameClient ../../_build/cardGame
	cp README ../../_build/cardGame

clean:
	$(RM) gameServer gameClient

.PHONY: install clean
