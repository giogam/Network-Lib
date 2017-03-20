CFLAGS:= -O3 -g -Wall -Werror -std=c++11
CC:= g++
RPATH:= -Wl,-R../../_build/.lib
LPATH:= -L../../_build/.lib
LIBS:= -lnetwork -lutils

all: admiServer admiClient

admiServer: admiServer.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

admiClient: admiClient.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

install:
	mkdir -p ../../_build/03-admiCtrl
	cp admiServer ../../_build/03-admiCtrl
	cp admiClient ../../_build/03-admiCtrl
	cp README ../../_build/03-admiCtrl

clean:
	$(RM) admiServer admiClient

.PHONY: install clean
