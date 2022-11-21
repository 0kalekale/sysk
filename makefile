CC = gcc -std=gnu11 -Wall -Wextra -Werror -Wno-format -pedantic -Iinclude

all: targetdir binall libktk kinet tests
	
include bin/bin.mk
include lib/libktk/ktk.mk
include lib/libkinet/kinet.mk

targetdir:
	mkdir -p target
	mkdir -p target/tests

tests: ktktest kinettest

clean:
	rm -rf target
