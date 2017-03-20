CFLAGS := -c -fPIC -O3 -g -Wall -Werror -std=c++11
CC := g++
NAME := thread
LIBS:= -lpthread
SOURCES:=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

lib: lib$(NAME).so

lib$(NAME).so: $(OBJECTS)
	$(CC) -shared $(OBJECTS) $(LIBS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $(LIBS) $< -o $@

clean:
	$(RM) *.o *.so*
