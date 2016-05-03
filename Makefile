CC=g++
CFLAGS=-std=c++11
OBJ=array.so complex.so fcntl.so fenv.so locale.so mqueue.so pthread.so random.so regex.so signal.so stdio.so stdlib.so termios.so thread.so time.so unistd.so
INC=/usr/local/include/lux
LIB=/usr/local/lib/lua/5.3

all: $(OBJ)

clean:
	rm $(OBJ)

install:
	mkdir $(INC)
	cp -t $(INC) src/*.hpp
	cp $(OBJ) $(LIB)

uninstall:
	rm $(INC)/*
	rmdir $(INC)
	rm $(addprefix $(LIB)/, $(OBJ))

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
