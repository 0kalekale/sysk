all: targetdir cat libktk ktktest
	
include bin/bin.mk
include lib/libktk/ktk.mk

targetdir:
	mkdir -p target
	mkdir -p target/tests
	
