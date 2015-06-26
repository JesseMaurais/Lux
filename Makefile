CC=g++
CFLAGS=-std=c++11
LDFLAGS=-llua -llux -ldl

all: liblux.so

test: test1 test2 test3 test4 test5

clean:
	rm liblux.so lux.o test1 test2 test3 test4 test5

install: liblux.so lxalloc.hpp lxstack.hpp lxthunk.hpp lxarray.hpp lxunion.hpp lxclass.hpp
	cp liblux.so /usr/local/lib
	mkdir -p /usr/local/include/lux
	cp *.hpp /usr/local/include/lux

uninstall:
	rm /usr/local/include/lux/*
	rmdir /usr/local/include/lux
	rm /usr/local/lib/liblux.so

liblux.so: lux.o
	$(CC) -shared -o liblux.so lux.o

lux.o: lux.cpp lxalloc.hpp lxstack.hpp lxthunk.hpp lxarray.hpp lxunion.hpp lxclass.hpp
	$(CC) $(CFLAGS) -fpic -c lux.cpp

test1: test1.cpp liblux.so
	$(CC) $(CFLAGS) -o test1 test1.cpp $(LDFLAGS)

test2: test2.cpp liblux.so
	$(CC) $(CFLAGS) -o test2 test2.cpp $(LDFLAGS)

test3: test3.cpp liblux.so
	$(CC) $(CFLAGS) -o test3 test3.cpp $(LDFLAGS)

test4: test4.cpp liblux.so
	$(CC) $(CFLAGS) -o test4 test4.cpp $(LDFLAGS)

test5: test5.cpp liblux.so
	$(CC) $(CFLAGS) -o test5 test5.cpp $(LDFLAGS) -lSDL2

