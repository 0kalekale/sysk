/*
unix cat but with syntax highlighting for:
- C files (*.c, *.h)
- shell scripts 
- assembly 
*/

#include <stdio.h>
#include <string.h>

#include <sysk/sysk.h>
#include <sysk/vtesc.h>

/*
	TODO:void master(int argc, char *argv) {
	get the build system working to link libsysk easily 
*/


// yes. 
void c_highlight(char *word) {
	if (strcmp(wordbuf, "int")) {
		printf(GRN"int"RST);
	}
	else {	
		printf("%s", wordbuf);
	}
}

void asm_highlight(char *word) {
	if (strcmp(wordbuf, "pop"))
	// and so on.
}
int main (int argc,  char *argv) {		
	
	if (argc<2) {
			// do something
 		}
	}
	
} 
