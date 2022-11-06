#ifndef _SIMPLESERVER_H_
#define _SIMPLESERVER_H_

#include <stdint.h> /* uint */

#include "../../include/apero.h"


#define MAX_CONEXIONES 1
#define MAX_DATASIZE   32


int iniciarServidor(servidor *, uint8_t);

uint8_t bucleConexiones(servidor *);

int enviarDatos(conexion *, uint8_t *, uint16_t);

#endif
