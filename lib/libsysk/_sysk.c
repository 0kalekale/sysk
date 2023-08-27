#include <sysk/sysk.h>
#include <sysk/mem.h>

/*
keep track of all memory allocated from the start of the program
*/

struct mem* _sysk_stack() {
	struct mem* stack = syskalloc(sizeof(struct mem)*1024);
	return stack; 
}

int _sysk_clean(struct mem* stack) {
	return syskfree(stack, sizeof(struct mem)*1024);
}
