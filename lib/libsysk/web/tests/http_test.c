#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysk/web/http.h>


int main() {

	server_info *server = malloc(1024);
	server->port = "80";
	server->host = "0kalekale.gitlab.io";

	http_connect(server);

	char *resp = get("/http.txt", server);
	printf("%s", resp);


	free(resp);
	free(server);

	return 0;
}
