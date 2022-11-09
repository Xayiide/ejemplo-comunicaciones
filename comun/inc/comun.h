#ifndef _COMUN_H_
#define _COMUN_H_

#define MAX_DATASIZE 32
#define LISTENING_ADDR "127.0.0.1"
#define LISTENING_PORT 42069

#include <stdio.h>  /* vprintf       */
#include <stdint.h> /* uint          */
#include <stdarg.h> /* variadic args */


void printColumns(uint8_t *, uint64_t, uint8_t);

void letrasAleatorias(uint8_t *, uint64_t);

int debug(const char *, ...);

#endif
