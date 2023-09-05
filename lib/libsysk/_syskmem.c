#include <sysk/_linux.h>

#define FAIL ((void *) -1)

void *_syskalloc(unsigned int size) {
	void *ret = _linux_mmap(0, size, 3, 34, 0, 0);
	if (ret == FAIL) {
		return FAIL; // TODO: should exit(-1) instead 
	}
	return ret;
}

int _syskfree(void *ptr, unsigned int size) {
	return _linux_munmap(ptr, size);
}

void *_syskrealloc(void *ptr, unsigned int tosize, unsigned int fromsize) {
	_syskfree(ptr, fromsize);
        void *ret = _linux_mmap(ptr, tosize, 3, 34, 0, 0);
	if (ret == FAIL) {
		return FAIL;
	}
	return ret;
}
