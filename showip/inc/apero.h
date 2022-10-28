#ifndef _APERO_H_
#define _APERO_H_

#include <sys/types.h>  /* struct addrinfo */
#include <sys/socket.h> /* struct addrinfo */
#include <netdb.h>      /* struct addrinfo */

void print_addrinfo(struct addrinfo *ai);


#endif /* _APERO_H_ */