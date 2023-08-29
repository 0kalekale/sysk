#include <sysk/sysk.h>
#include <kinet/http.h>
#include <stdio.h>

#define REPO "https://gitlab.com/0kalekale/binaries/-/raw/main/README"

#define BUFFSIZE sizeof(char)*1024*16

int master(void) {
	
	
	char *buf = syskalloc(BUFFSIZE);
	kinet_get(rep, buf, sizeof(BUFFSIZE);

	/*
	a sample on what the repo looks like:
	
	repo_url = {base url}

	pkg1 = {name.ver.ver.ver-hashsum.tar}
	pkg2 = {name.ver.ver.ver-hashsum.tar}

	a sample on how the pkg manager will be used:

	$ syskpkg u syskpkg	[update core_syskpkg]
	$ syskpkg u		[updates everything]
	$ syskpkg i cunnywm 	[install cunnywm]
	$ syskpkg r cunnywm 	[remove cunnywm]
	
	everything will be installed to .local/sysk/bin 
	libraries will be linked statically 

	process
	parse args->get repo->check for package in repo
	-> if does not exist, exit
	-> if exists download the tar  
	-> unpack the archive 
	-> install it to .local/sysk/bin
	*/

	// TODO: implement a https client function kinet_get();
	// TODO: repo parser 
	// TODO: use libtar 

}
