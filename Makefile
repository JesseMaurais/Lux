CC=g++
CFLAGS=-std=c++11
SRC=lux.cpp lxalloc.hpp lxstack.hpp lxthunk.hpp lxarray.hpp lxclass.hpp

all: liblux.so

clean:
	rm *.so

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

cstdio: $(SRC) cstdio.cpp
	$(CC) $(CFLAGS) -shared -o cstdio.so -fpic cstdio.cpp -llux

cstring: $(SRC) cstring.cpp
	$(CC) $(CFLAGS) -shared -o cstring.so -fpic cstring.cpp -llux

unistd: $(SRC) unistd.cpp
	$(CC) $(CFLAGS) -shared -o unistd.so -fpic unistd.cpp -llux

random: $(SRC) random.cpp
	$(CC) $(CFLAGS) -shared -o random.so -fpic random.cpp -llux

array: $(SRC) array.cpp
	$(CC) $(CFLAGS) -shared -o array.so -fpic array.cpp -llux

test: $(SRC) test.cpp
	$(CC) $(CFLAGS) -shared -o test.so -fpic test.cpp -llux

