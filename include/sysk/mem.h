/*
system K's malloc replacement for linux x86_64
*/

extern void* _syskalloc(unsigned int);
extern int _syskfree(void* ptr, unsigned int size);
