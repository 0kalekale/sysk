/*
useful typedefs since im not using libc
*/
typedef int ssize_t;
typedef unsigned int size_t ;
typedef unsigned short umode_t; 
typedef int fd_t;

// syscall wrapper (defined in asm)
void *_syscall_linux(signed int syscall, ...);

// sys_mmap
void *_linux_mmap(unsigned long addr, unsigned long len, unsigned long prot, unsigned long flags, fd_t fd, unsigned long off) {
	return _syscall_linux(9, addr, len, prot, flags, fd, off);
}

