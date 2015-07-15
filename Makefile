CC=g++
CFLAGS=-std=c++11
SRC=lux.cpp lxalloc.hpp lxstack.hpp lxthunk.hpp lxarray.hpp lxclass.hpp

all: liblux.so

clean:
	rm liblux.so array.so random.so test.so

install: liblux.so $(SRC)
	cp liblux.so /usr/local/lib
	mkdir -p /usr/local/include/lux
	cp *.hpp /usr/local/include/lux

uninstall:
	rm /usr/local/include/lux/*
	rmdir /usr/local/include/lux
	rm /usr/local/lib/liblux.so

liblux.so: $(SRC)
	$(CC) $(CFLAGS) -shared -o liblux.so -fpic lux.cpp

random: $(SRC) random.cpp
	$(CC) $(CFLAGS) -shared -o random.so -fpic random.cpp -llux

array: $(SRC) array.cpp
	$(CC) $(CFLAGS) -shared -o array.so -fpic array.cpp -llux

test: $(SRC) test.cpp
	$(CC) $(CFLAGS) -shared -o test.so -fpic test.cpp -llux

