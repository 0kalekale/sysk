/*
build system for sysk
will be slow, but easy to use

-------------------
example kcompile.kc
-------------------

cc=gcc
target_dir=target
inc_dir=include
std=c11

bin: 
	exec: hxd
	source: bin/core/hxd.c 
	libsysk: no
	libkmk: no
	libkinet: no
	static: yes  

lib:
	obj: libsysk.a
	source_dir: lib/libsysk
	source: _entry.s _linux/_syscall_linux.s _linux/_syscalls.c  _linux/_void_syscalls.c _sysk.c  _syskmem.c
	libc: no 
*/
