#include <sysk/sysk.h>
#include <sysk/mem.h>

#pragma GCC diagnostic ignored "-Wint-conversion"

/*
keep track of all memory allocated from the start of the program
*/

// master stack, unless the programmer wants to create a new one
// they will be using this stack, and this only 
struct stack* _master_stack_1024_16;

// _sysk_stack() and _sysk_clean() should only be use once in a program!
// to create a custom stack, there will be a different frontend 
int _sysk_stack() {
	_master_stack_1024_16 = (struct stack*) _syskalloc(sizeof(struct stack));
	if (_master_stack_1024_16 == FAIL) {
		return FAIL;
	}
	_master_stack_1024_16->ptr = (struct mem*) _syskalloc(sizeof(struct mem)*1024);
	if (_master_stack_1024_16->ptr == FAIL) {
		return FAIL;
	}
	else { 
		_master_stack_1024_16->stack_counter = 0;
		return 0; 
	}
}

int _sysk_clean() {
	return _syskfree(_master_stack_1024_16, sizeof(struct mem)*1024); // leaks, yet to free the pointers in the stack
}

// _syskalloc() is the private internal function 
// programs using libsysk should not be using that
// syskalloc() and syskfree() are the frontends
// _syskalloc() and _syskfree() are the backends
void* syskalloc(unsigned int size)  {
	void *retptr = _syskalloc(size);
	
	if (retptr == FAIL) {
		return FAIL; 
	}
	
	_master_stack_1024_16->ptr[_master_stack_1024_16->stack_counter].ptr = retptr;
	_master_stack_1024_16->ptr[_master_stack_1024_16->stack_counter].size = size; 
	
	_master_stack_1024_16->stack_counter += 1;
	/*
	FIXME: instead of using a stack_counter, which is fine for a small scale
	program, have syskalloc() find a free block in the _master_stack_1024_16 
	and then allocate it there.
	*/
	return retptr;
}

// maybe slow
int syskfree(void *ptr) {
	for(int i = 0; i<1024; i++) {
		if (ptr == _master_stack_1024_16->ptr[i].ptr) {
			return _syskfree(ptr, _master_stack_1024_16->ptr[i].size);
		}
	}
	return FAIL;
}


// reallocate syskalloc()'ed block
void* syskrealloc(void *ptr, unsigned int size) {
	void *retptr;
	void *tmp = _syskalloc(size);

	syskmemcpy(tmp, ptr, size);

	for(int i = 0; i<1024; i++) {
		if (ptr == _master_stack_1024_16->ptr[i].ptr) {
			retptr = _syskrealloc(ptr, size, _master_stack_1024_16->ptr[i].size);
			if (retptr == FAIL) {
				return FAIL;
			}
			_master_stack_1024_16->ptr[i].size = size;
		}
	}
	
	syskmemcpy(retptr, tmp, size);
	_syskfree(tmp, size);

	return retptr;
}
