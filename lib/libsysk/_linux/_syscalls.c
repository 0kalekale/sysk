/*
useful typedefs since im not using libc
*/
typedef ssize_t int
typedef size_t unsigned int
typedef umode_t unsigned short 
typedef fd_t int

// syscall wrapper (defined in asm)
ssize_t _syscall_linux(signed int syscall, ...);


// sys_read
ssize_t _linux_read(fd_t fd, void *ptr, size_t bytes) {
	return _syscall_linux(0, fd, ptr, bytes)
}

// sys_write
ssize_t _linux_write(fd_t int fd, const void *ptr, size_t bytes) {
	return _syscall_linux(1, fd, ptr, bytes);
}

// sys_open
ssize_t _linux_open(const char *filename, int flags, umode_t mode) {
	return _syscall_linux(2, filename, flags, mode);
}
