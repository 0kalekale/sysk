/*
	sysk hello world example
	with demonstrations of syskalloc() and syskfree()

	to build (run from libsysk dir, build system coming soon lol)
	$ gcc example/sysk_helloworld.c _entry.s _sysk.c _syskmem.c _linux/_syscalls.c _linux/_syscall_linux.s -I../../include _linux/_void_syscalls.c  -o test -g
	inb4 >holy fucking shit what the fuck is wrong with you just use a build system retard  
*/

#include <sysk/sysk.h>
#include <sysk/mem.h>
#include <stdio.h>

void master(int argc, char *argv) {
	printf("hello world\n");

	char* ptr = (char*)syskalloc(100);
	printf("%x\n", ptr);
	ptr[0] = 'A';
	ptr = syskrealloc(ptr, 200); // FIXME: ptr[0] should not be affected after reallocing  
	printf("%x\n", ptr);
	ptr[120] = 'B';
	printf("%c %c", ptr[0], ptr[120]);
	syskfree(ptr);
}
