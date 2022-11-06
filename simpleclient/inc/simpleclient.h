#ifndef _SIMPLECLIENT_H_
#define _SIMPLECLIENT_H_

#include <stdint.h> /* uint8_t */

#include "../../include/apero.h"

int enviarDatos(cliente *, uint8_t *, uint16_t);

int recibirDatos(cliente *, uint8_t *, uint16_t);

#endif
