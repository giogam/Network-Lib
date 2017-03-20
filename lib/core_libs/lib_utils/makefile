CFLAGS := -c -fPIC -O3 -g -Wall -Werror -std=c++11
CC := g++
NAME := utils
SOURCES:=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

lib: lib$(NAME).so

lib$(NAME).so: $(OBJECTS)
	$(CC) -shared $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) *.o *.so*
