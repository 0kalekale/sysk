libktk: 
	$(CC) -c lib/libktk/ktk.c -Iinclude -o target/libktk.o
	gcc -c lib/libktk/ktk.c -Iinclude -o target/libktk.o
	ar rcs target/libktk.a target/libktk.o 
ktktest:
	gcc lib/libktk/tests/main.c lib/libktk/tests/renderer.c target/libktk.a -Iinclude -Ilib/libktk/tests/ -o target/tests/ktksample `sdl2-config --libs` -lGL -lm

