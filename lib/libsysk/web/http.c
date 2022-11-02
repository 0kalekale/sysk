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
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <sysk/web/http.h>

int sock;

int http_connect(server_info *server) {

	struct addrinfo hints = {0}, *addrs;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo(server->host, server->port, &hints, &addrs);

	for(struct addrinfo *addr = addrs; addr != NULL; addr = addr->ai_next) {
        if(!(sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol)))
			return 1;
        if (connect(sock, addr->ai_addr, addr->ai_addrlen) == 0)
            break;
		sock = -1; // could not connect.
    }
    freeaddrinfo(addrs);
	return 0;
}

int get(char *path, char *resp_buffer, int buffsize, server_info *server) {

	// Request builder
	char request[1024];
	sprintf(request, "GET %s HTTP/1.1\r\n\
User-Agent: libsys/0.1\r\n\
Host: %s\r\n\r\n",
					path, server->host);
	//TODO: HTTPs
	//TODO: Custom UserAgent

	if(!send(sock, request, strlen(request), 0)) {
		fprintf(stdout, "[error] could not send message");
		return 1;
	}

	if(!read(sock, resp_buffer, buffsize)) {
		fprintf(stdout, "[error] could not read response into buffer");
		return 1;
	}
	return 0;

}
