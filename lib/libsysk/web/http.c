#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <sysk/web/http.h>

int get(uint16_t port, const char* addr) {
	int sock, sockfd;
	if (!(sock = socket(AF_INET, SOCK_STREAM, 0))) {
		fprintf(stderr,  "[error] : could not create a socket");
		return 1;
	}

	if (!inet_addr(addr)) {
		fprintf(stderr, "[error] : not a valid address");
		return 1;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if(!(sockfd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))) {
		fprintf(stderr, "[error] : could not create connection");
		return 1;
	}
	return 0;
}
