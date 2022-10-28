/*
    sysk/web/http: A simple http client server library.
    Copyright (C) 2022 0kalekale

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <sysk/web/http.h>

int sock;
int sockfd;

int http_connect(uint16_t port, const char* addr) {

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

int get(char *message, void* resp_buffer) {

	if(!message) {
		fprintf(stderr, "[error] null pointer *message");
		return 1;
	}

	if(!send(sock, message, strlen(message), 0)) {
		fprintf(stderr, "[error] could not send message");
		return 1;
	}

	// TODO: read bytes one by one and append to butter.
	if(!read(sock, resp_buffer, sizeof(resp_buffer))) /* VERY bad way to do this */{
		fprintf(stderr, "[error] could not read response into buffer");
		return 1;
	}

}
