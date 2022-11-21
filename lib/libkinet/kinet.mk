kinet:
	rustc lib/libkinet/lib.rs --crate-type=staticlib -o target/libkinet.a

kinettest:
	gcc lib/libkinet/test.c target/libkinet.a -pthread -ldl -o target/tests/placeholder 
