CC=g++
CFLAGS=-std=c++11
SRC=lxalloc.hpp lxstore.hpp lxstack.hpp lxthunk.hpp lxarray.hpp lxclass.hpp lxbuffs.hpp lxerror.hpp lxcmath.hpp lxtools.hpp
OBJ=array.so lapacke.so cblas.so cstdlib.so cstdio.so cstring.so csignal.so ctime.so clocale.so cfenv.so unistd.so termios.so random.so thread.so test.so

all: $(OBJ)

clean:
	rm $(OBJ)

install: $(SRC)
	mkdir -p /usr/local/include/lux
	cp -t /usr/local/include/lux $(SRC)

uninstall:
	rm /usr/local/include/lux/*
	rmdir /usr/local/include/lux

thread.so: thread.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -pthread

cfenv.so: cfenv.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -lm

cblas.so: cblas.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -lblas

lapacke.so: lapacke.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -lblas -llapacke

%.so: %.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $<

