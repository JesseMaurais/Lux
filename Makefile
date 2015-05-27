CC=g++
CCFLAGS=-std=c++11 -g
LDFLAGS=-llua5.2

all: test1 test2 test3 test4 test5

lux.o: lux.cpp lxalloc.hpp lxstack.hpp lxthunk.hpp lxarray.hpp lxunion.hpp lxclass.hpp
	$(CC) $(CCFLAGS) -c lux.cpp

test1: test1.cpp lux.o
	$(CC) $(CCFLAGS) -o test1 lux.o test1.cpp $(LDFLAGS)

test2: test2.cpp lux.o
	$(CC) $(CCFLAGS) -o test2 lux.o test2.cpp $(LDFLAGS)

test3: test3.cpp lux.o
	$(CC) $(CCFLAGS) -o test3 lux.o test3.cpp $(LDFLAGS)

test4: test4.cpp lux.o
	$(CC) $(CCFLAGS) -o test4 lux.o test4.cpp $(LDFLAGS)

test5: test5.cpp lux.o
	$(CC) $(CCFLAGS) -o test5 lux.o test5.cpp $(LDFLAGS) -lSDL2

clean:
	rm lux.o test1 test2 test3 test4 test5
