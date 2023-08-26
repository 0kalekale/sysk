/*
system K's malloc replacement for linux x86_64
*/

extern void* syskalloc(unsigned int);
extern int syskfree(void* ptr, unsigned int size);
