#include <sysk/sysk.h>

/*
 Abandoned for now, until a figure out a better way to write a resource allocator.
 */
struct mem *syskstacknew(unsigned int n); {
	struct stack *ptr = (struct stack*) _syskalloc(sizeof(struct stack));
	if (ptr == FAIL) {
		return FAIL;
	}
	ptr.ptr = (struct mem*) _syskalloc(sizeof(struct mem)*n);
	if (ptr.ptr == FAIL) {
			return FAIL
	}
	else {
			return ptr; 
	}
}

void *syskstackclean(struct stack *st, unsigned int n) {
	_syskfree((void)st, n); // leaks, yet to free the ptr.ptr[i].ptr(s) (what the fuck am i doing)
}

void *syskstackalloc(struct  stack *st, unsigned int size) {
	void *retptr = _syskalloc(size);
	
	if (retptr == FAIL) {
		return FAIL; 
	}
	
	st.ptr[stack_counter].ptr = retptr;
	st.ptr[stack_counter].size = size; 
	
	st.stack_counter += 1;
	/*
	FIXME: instead of using a stack_counter, which is fine for a small scale
	program, have syskalloc() find a free block in the _master_stack_1024_16 
	and then allocate it there.

	*/
	return retptr;
} 

void syskstackfree(struct stack *st, void *ptr) {
	//
}

void syskstackrealloc(struct mem *stack, void *ptr, unsigned int size) {
	/*
	...
	*/
}
