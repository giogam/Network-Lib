CFLAGS := -c -fPIC -O3 -g -Wall -Werror -std=c++11
CC := g++
NAME := routines
LIBS:= -lnetwork -lutils -lthread -lpthread
RPATH:= -Wl,-R../../../_build/.lib
LPATH:= -L../../../_build/.lib
SOURCES:=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

lib: lib$(NAME).so

lib$(NAME).so: $(OBJECTS)
	$(CC) -shared $(OBJECTS) $(LPATH) $(RPATH) $(LIBS) -o $@  

%.o: %.cpp
	$(CC) $(CFLAGS) $(LIBS) $< -o $@

clean:
	$(RM) *.o *.so*
