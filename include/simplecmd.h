#ifndef _SIMPLECMD_H
#define _SIMPLECMD_H_

#include <stdint.h> /* uints */

#define RESP_SALUDO_LEN 8

#define CMD_TAM    1

#define CMD_SALUDO 0x41

#define CMD_SUMA   0x42
#define SUMA_TAM   2

#define CMD_RESTA  0x43
#define RESTA_TAM  2

#define CMD_MULT   0x44
#define MULT_TAM   2

#define CMD_NONE   0x45

const uint8_t RESP_SALUDO[RESP_SALUDO_LEN] = {
        CMD_SALUDO, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
};

#endif

