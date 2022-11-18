all: targetdir cat libktk ktktest
	
include bin/cat/cat.mk
include lib/libktk/ktk.mk

targetdir:
	mkdir -p target
	mkdir -p target/tests
	
