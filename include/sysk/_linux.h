/*
useful typedefs since im not using libc
*/
typedef int ssize_t;
typedef unsigned int size_t ;
typedef unsigned short umode_t; 
typedef int fd_t;

// syscall wrapper (defined in asm)
ssize_t _syscall_linux(signed int syscall, ...);
// sys_read
ssize_t _linux_read(fd_t fd, void *ptr, size_t bytes);
// sys_write
ssize_t _linux_write(fd_t fd, const void *ptr, size_t bytes); 
// sys_open
ssize_t _linux_open(const char *filename, int flags, umode_t mode);
// sys_mmap
void* _linux_mmap(void *addr, unsigned long len, unsigned long prot, unsigned long flags, fd_t fd, unsigned long off);
// sys_munmap
ssize_t _linux_munmap(void* addr, size_t len);
