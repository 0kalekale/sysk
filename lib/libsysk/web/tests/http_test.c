#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysk/web/http.h>

#define BUFFSIZE 1024*50

int main() {

	server_info *server = malloc(1024);
	server->port = "80";
	server->host = "0kalekale.gitlab.io";

	http_connect(server);

	char *buffer = malloc(BUFFSIZE);

	get("/http.txt", buffer, BUFFSIZE, server);
	printf("%s", buffer);

	free(buffer);
	free(server);

	return 0;
}
