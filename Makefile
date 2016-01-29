CC=g++
CFLAGS=-std=c++11 -I/usr/include/lua5.3
SRC=lux.hpp lxalloc.hpp lxstore.hpp lxstack.hpp lxthunk.hpp lxchars.hpp lxarray.hpp lxclass.hpp lxbuffs.hpp lxdebug.hpp lxerror.hpp lxtools.hpp
OBJ=array.so blas.so complex.so fcntl.so fenv.so lapack.so locale.so mqueue.so pthread.so random.so regex.so signal.so stdio.so stdlib.so termios.so thread.so time.so unistd.so

all: $(OBJ)

clean:
	rm $(OBJ)

install: $(SRC)
	mkdir -p /usr/local/include/lux
	cp -t /usr/local/include/lux $(SRC)
	lua install.lua $(OBJ)

uninstall:
	rm /usr/local/include/lux/*
	rmdir /usr/local/include/lux

%.so: src/%.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $<

thread.so: src/thread.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -pthread

pthread.so: src/pthread.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -pthread

mqueue.so: src/mqueue.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -lrt

time.so: src/time.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -lrt

blas.so: src/blas.cpp src/cblas.hpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -lblas

lapack.so: src/lapack.cpp src/lapacke.hpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -llapacke

