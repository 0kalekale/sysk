libktk: 
	gcc -c lib/libktk/ktk.c -Iinclude -o target/libktk.o

ktktest:
	gcc -c lib/libktk/tests/sample.c -Iinclude -o target/tests/ktksample.o
	gcc -o target/tests/ktksample target/tests/ktksample.o target/libktk.o -lX11 -lxcb
