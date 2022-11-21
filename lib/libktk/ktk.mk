libktk: 
	$(CC) -c lib/libktk/ktk.c -Iinclude -o target/libktk.o

ktktest:
	$(CC) -c lib/libktk/tests/sample.c -Iinclude -o target/tests/ktksample.o
	$(CC) -o target/tests/ktksample target/tests/ktksample.o target/libktk.o -lX11 -lxcb
