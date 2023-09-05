#include <sysk/sysk.h>
#include <sysk/stack.h>

struct mem *syskstacknew(unsigned int n); {
	mem *ret = _syskalloc(n * sizeof(mem));
	return ret;	
}

void *syskstackclean(struct mem *stack, unsigned int n) {
	_syskfree((void)stack, n);
}

void *syskstackalloc(struct  mem *stack, unsigned int size) {
	/*
	look for free space in stack 
	and then alloc and return  
	no counter hack this time
	*/	
} 

void syskstackfree(struct mem *stack, void *ptr) {
	/*
	...
	*/
}

void syskstackrealloc(struct mem *stack, void *ptr, unsigned int size) {
	/*
	...
	*/
}
