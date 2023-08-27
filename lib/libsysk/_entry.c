#include <sysk/sysk.h>

/*
general template for all sysk program, a hello world would look like:

----hello_sysk.c----
#include <sysk/sysk.h>
#include <stdio.h>

void master(void) {
	printf("hello world");
}
--------------------
$ gcc hello_sysk.c libsysk.o 
*/

int main(int argc, char *argv) {
	struct mem* stack = _sysk_stack();
	master(void);
	return _sysk_clean(stack);
}
