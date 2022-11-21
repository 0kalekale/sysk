binall: cat hxd date uname tty whoami

cat: 
	rustc bin/cat/cat.rs -o target/cat
date:
	$(CC) bin/date/date.c -o target/k_date
uname:
	$(CC) bin/uname/uname.c -o target/k_uname
tty:
	$(CC) bin/tty/tty.c -o target/k_tty
whoami:
	$(CC) bin/whoami/whoami.c -o target/k_whoami
hxd:
	$(CC) bin/hxd/hxd.c -o target/hxd
