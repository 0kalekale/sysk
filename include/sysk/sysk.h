#pragma once

void master();

struct mem {
	void *ptr;
	unsigned int size; 
};

//struct mem* _master_stack_1024_16;

int _sysk_stack();
int _sysk_clean();

void* syskalloc(unsigned int size);
int syskfree(void *ptr);

