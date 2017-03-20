CFLAGS:= -O3 -g -Wall -Werror -std=c++11
CC:= g++
RPATH:= -Wl,-R../../_build/.lib
LPATH:= -L../../_build/.lib
LIBS:= -lnetwork -lutils

all: dsServer dsClient

dsServer: dsServer.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

dsClient: dsClient.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

install:
	mkdir -p ../../_build/04-DS
	cp dsServer ../../_build/04-DS
	cp dsClient ../../_build/04-DS
	cp phone_db.pdb ../../_build/04-DS
	cp README ../../_build/04-DS

clean:
	$(RM) dsServer dsClient

.PHONY: install clean
