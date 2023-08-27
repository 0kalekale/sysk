#include <sysk/sysk.h>
#include <sysk/mem.h>

#pragma GCC diagnostic ignored "-Wint-conversion"

/*
keep track of all memory allocated from the start of the program
*/

// master stack, unless the programmer wants to create a new one
// they will be using this stack, and this only 
struct mem* _master_stack_1024_16;
// stack counter
int stack_counter = 0; 

// _sysk_stack() and _sysk_clean() should only be use once in a program!
// to create a custom stack, there will be a different frontend 
int _sysk_stack() {
	_master_stack_1024_16 = (struct mem*) _syskalloc(sizeof(struct mem)*1024);
	if (_master_stack_1024_16 == (void*)-1) {
		return -1;
	}
	else { return 0; }
}

int _sysk_clean() {
	return _syskfree(_master_stack_1024_16, sizeof(struct mem)*1024);
}

// _syskalloc() is the private internal function 
// programs using libsysk should not be using that
// syskalloc() and syskfree() are the frontends
// _syskalloc() and _syskfree() are the backends
void* syskalloc(unsigned int size)  {
	void *retptr = _syskalloc(size);
	
	if (retptr == (void*)-1) {
		return -1; // TODO: #define -1 as SYSKERR
	}
	
	_master_stack_1024_16[stack_counter].ptr = retptr;
	_master_stack_1024_16[stack_counter].size = size; 
	
	stack_counter += 1;
	return retptr;
}

// maybe slow
int syskfree(void *ptr) {
	for(int i = 0; i<1024; i++) {
		if (ptr == _master_stack_1024_16[i].ptr) {
			return _syskfree(ptr, _master_stack_1024_16[i].size);
		}
	}
	return -1;
} 
