CC=g++
CFLAGS=-std=c++11
SRC=lux.hpp lxalloc.hpp lxstore.hpp lxstack.hpp lxthunk.hpp lxarray.hpp lxclass.hpp lxbuffs.hpp lxerror.hpp lxtools.hpp
OBJ=array.so complex.so cblas.so lapacke.so random.so pthread.so csignal.so cstdlib.so cstdio.so clocale.so cfenv.so unistd.so 

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

cblas.so: src/cblas.cpp src/cblas.hpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -lblas

lapacke.so: src/lapacke.cpp src/lapacke.hpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -llapacke

pthread.so: src/pthread.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -pthread

