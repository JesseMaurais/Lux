CC=g++
CFLAGS=-std=c++11
SRC=lux.hpp lxalloc.hpp lxarray.hpp lxbuffs.hpp lxchars.hpp lxclass.hpp lxdebug.hpp lxerror.hpp lxstack.hpp lxstore.hpp lxthunk.hpp lxtools.hpp
OBJ=array.so complex.so fcntl.so fenv.so locale.so mqueue.so pthread.so random.so regex.so signal.so stdio.so stdlib.so termios.so thread.so time.so unistd.so
INC=/usr/local/include/lux
LIB=/usr/local/lib/lua/5.3

all: $(OBJ)

clean:
	rm $(OBJ)

install:
	mkdir -p $(INC)
	cp -t $(INC) $(addprefix src/, $(SRC))
	mkdir -p $(LIB)
	cp $(OBJ) $(LIB)

uninstall:
	rm $(addprefix $(INC)/, $(SRC))
	rmdir $(INC)
	rm $(addprefix $(LIB)/, $(OBJ))

%.so: src/%.cpp
	$(CC) $(CFLAGS) -shared -o $@ -fpic $<

thread.so: src/thread.cpp
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -pthread

pthread.so: src/pthread.cpp
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -pthread

mqueue.so: src/mqueue.cpp
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -lrt

time.so: src/time.cpp
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -lrt

