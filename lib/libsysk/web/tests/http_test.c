#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysk/web/http.h>

#define MSG "GET /http.txt HTTP/1.1\r\nUser-Agent: libsysk/0.1\r\nHOST:0kalekale.gitlab.io\r\n\r\n"
#define BUFFSIZE 1024*50
int main() {

	http_connect("80", "0kalekale.gitlab.io");

	char *buffer = malloc(BUFFSIZE);
	get(MSG, buffer, BUFFSIZE);
	printf("%s", buffer);

	return 0;
}
