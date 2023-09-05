#pragma once

void master(int argc, char *argv);

struct mem {
	void *ptr;
	unsigned int size; 
};

int _sysk_stack();
int _sysk_clean();

void* syskalloc(unsigned int size);
int syskfree(void *ptr);
void* syskrealloc(void *ptr, unsigned int size);
void syskmemcpy(char *dest, char *src, unsigned int size);
