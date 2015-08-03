CC=g++
CFLAGS=-std=c++11
SRC=lxalloc.hpp lxstore.hpp lxstack.hpp lxthunk.hpp lxarray.hpp lxclass.hpp lxtable.hpp lxbuffs.hpp lxerror.hpp
OBJ=liblux.so array.so cstdlib.so cstdio.so cstring.so csignal.so ctime.so clocale.so cfenv.so unistd.so termios.so random.so thread.so mutex.so test.so

all: $(OBJ)

clean:
	rm $(OBJ)

install: liblux.so $(SRC)
	cp liblux.so /usr/local/lib
	mkdir -p /usr/local/include/lux
	cp *.hpp /usr/local/include/lux

uninstall:
	rm /usr/local/include/lux/*
	rmdir /usr/local/include/lux
	rm /usr/local/lib/liblux.so

liblux.so: lux.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $<

thread.so: thread.cpp
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -llux -pthread

mutex.so: mutex.cpp
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -llux -pthread

cfenv.so: cfenv.cpp
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -llux -lm

%.so: %.cpp $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -llux

