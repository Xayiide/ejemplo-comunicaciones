#include <stdio.h>
#include <sys/types.h>  /* getaddrinfo */
#include <sys/socket.h> /* getaddrinfo */
#include <netdb.h>      /* getaddrinfo */
#include <string.h>     /* memset      */

#include "../include/apero.h"


int main(int argc, char** argv) {
	struct addrinfo hints, *res;
	int status;

	
	if (argc != 2) {
		fprintf(stderr, "uso: showip hostname\n");
		return 1;
	}


	memset(&hints, 0, sizeof hints);
	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(argv[1], NULL, &hints, &res);
	if (status != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 2;
	}

	print_addrinfo(res);

	freeaddrinfo(res);
	
	return 0;
}

