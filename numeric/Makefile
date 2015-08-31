NUMERIC=blas.so lapack.so

all: $(NUMERIC)

clean:
	rm $(NUMERIC)

blas.so: blas.cpp
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -lblas

lapack.so: lapack.cpp
	$(CC) $(CFLAGS) -shared -o $@ -fpic $< -llapacke

