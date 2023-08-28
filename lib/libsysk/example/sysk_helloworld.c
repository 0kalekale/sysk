/*
	sysk hello world example
	with demonstrations of syskalloc() and syskfree()

	to build (run from libsysk dir, build system coming soon lol)
	$ gcc example/sysk_helloworld.c _entry.c _sysk.c _syskalloc.s _syskfree.s -I../../include 
*/

#include <sysk/sysk.h>
#include <sysk/mem.h>
#include <stdio.h>

void master(int argc, char *argv) {
	printf("hello world\n");

	char* ptr = (char*)syskalloc(100);
	syskfree(ptr);
}
