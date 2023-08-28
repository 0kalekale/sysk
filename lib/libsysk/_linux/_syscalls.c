#include <sysk/_linux.h>

// sys_read
ssize_t _linux_read(fd_t fd, void *ptr, size_t bytes) {
	return _syscall_linux(0, fd, ptr, bytes);
}

// sys_write
ssize_t _linux_write(fd_t fd, const void *ptr, size_t bytes) {
	return _syscall_linux(1, fd, ptr, bytes);
}

// sys_open
ssize_t _linux_open(const char *filename, int flags, umode_t mode) {
	return _syscall_linux(2, filename, flags, mode);
}

// sys_munmap
ssize_t _linux_munmap(void* addr, size_t len) {
	return _syscall_linux(11, addr, len);
}
