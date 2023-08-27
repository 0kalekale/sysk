void master(void);

struct mem {
	void *ptr;
	unsigned int size; 
};

struct mem* _sysk_stack();
int _sysk_clean(struct mem* stack);
