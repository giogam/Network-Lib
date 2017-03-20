CFLAGS:= -O3 -g -Wall -Werror -std=c++11
CC:= g++
RPATH:= -Wl,-R../../_build/.lib
LPATH:= -L../../_build/.lib
LIBS:= -lnetwork -lutils -lthread -lroutines -lsession

all: chatServer chatClient

chatServer: chatServer.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

chatClient: chatClient.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

install:
	mkdir -p ../../_build/07-chat
	cp chatServer ../../_build/07-chat
	cp chatClient ../../_build/07-chat
	cp README ../../_build/07-chat

clean:
	$(RM) chatServer chatClient

.PHONY: install clean
