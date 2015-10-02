CC=g++
CFLAGS=-std=c++11
SRC=lux.hpp lxalloc.hpp lxstore.hpp lxstack.hpp lxthunk.hpp lxchars.hpp lxarray.hpp lxclass.hpp lxbuffs.hpp lxerror.hpp lxtools.hpp
OBJ=array.so complex.so fcntl.so fenv.so locale.so mqueue.so pthread.so random.so regex.so signal.so stdio.so stdlib.so termios.so thread.so time.so unistd.so
DIR=numeric

.EXPORT_ALL_VARIABLES:
.PHONY: $(DIR)

all: $(OBJ) $(DIR)

clean:
	rm $(OBJ)
	for dir in $(DIR); do $(MAKE) -C $$dir clean; done

install: $(SRC)
	mkdir -p /usr/local/include/lux
	cp -t /usr/local/include/lux $(SRC)

uninstall:
	rm /usr/local/include/lux/*
	rmdir /usr/local/include/lux

$(DIR):
	$(MAKE) -C $@

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

