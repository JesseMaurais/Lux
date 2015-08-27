CC=g++
CFLAGS=-std=c++11
SRC=lux.hpp lxalloc.hpp lxstore.hpp lxstack.hpp lxthunk.hpp lxchars.hpp lxarray.hpp lxclass.hpp lxbuffs.hpp lxerror.hpp lxtools.hpp
OBJ=array.so complex.so random.so regex.so thread.so pthread.so cmath.so cfenv.so csignal.so cstdio.so cstdlib.so cstring.so clocale.so unistd.so

all: $(OBJ)

clean:
	rm $(OBJ)

install: $(SRC)
	mkdir -p /usr/local/include/lux
	cp -t /usr/local/include/lux $(SRC)

uninstall:
	rm /usr/local/include/lux/*
	rmdir /usr/local/include/lux

%.so: src/%.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $<

cmath.so: src/cmath.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic -fopenmp $<

thread.so: src/thread.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -pthread

pthread.so: src/pthread.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -pthread

