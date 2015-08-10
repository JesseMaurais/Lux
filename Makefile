CC=g++
CFLAGS=-std=c++11
SRC=lxalloc.hpp lxstore.hpp lxstack.hpp lxthunk.hpp lxarray.hpp lxclass.hpp lxbuffs.hpp lxerror.hpp lxtools.hpp
OBJ=array.so complex.so cblas.so lapacke.so random.so thread.so cstdlib.so cstdio.so cstring.so csignal.so ctime.so clocale.so cfenv.so unistd.so termios.so test.so

all: $(OBJ)

clean:
	rm $(OBJ)

install: $(SRC)
	mkdir -p /usr/local/include/lux
	cp -t /usr/local/include/lux $(SRC)

uninstall:
	rm /usr/local/include/lux/*
	rmdir /usr/local/include/lux

%.so: %.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $<

cfenv.so: cfenv.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -lm

cblas.so: cblas.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -lblas

lapacke.so: lapacke.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -lblas -llapacke

thread.so: thread.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -pthread

