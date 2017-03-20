CFLAGS:= -O3 -g -Wall -Werror -std=c++11
CC:= g++
RPATH:= -Wl,-R../../_build/.lib
LPATH:= -L../../_build/.lib
LIBS:= -lnetwork -lutils

all: ftpServer ftpClient

ftpServer: ftpServer.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

ftpClient: ftpClient.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LPATH) $(RPATH)

install:
	mkdir -p ../../_build/05-FTP
	cp ftpServer ../../_build/05-FTP
	cp ftpClient ../../_build/05-FTP
	cp -rf files ../../_build/05-FTP
	cp README ../../_build/05-FTP

clean:
	$(RM) ftpServer ftpClient

.PHONY: install clean
