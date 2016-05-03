CC=g++
CFLAGS=-std=c++11
OBJ=array.so complex.so fcntl.so fenv.so locale.so mqueue.so pthread.so random.so regex.so signal.so stdio.so stdlib.so termios.so thread.so time.so unistd.so

all: $(OBJ)

clean:
	rm $(OBJ)

install:
	mkdir /usr/local/include/lux
	cp -t /usr/local/include/lux src/*.hpp

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
