
#include "../include/comun.h"
#include "../include/apero.h"
#include "inc/simpleclient.h"

#include <stdio.h> /* printf */

uint8_t datos_enviar[MAX_DATASIZE] = {
    0x42, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
};

int main(void) {
	cliente *c = crearCliente(LISTENING_ADDR, LISTENING_PORT);
	ssize_t tam_recb = 0;
	uint8_t datos_recb[MAX_DATASIZE];


	if (c == NULL) {
		printf("Error creando cliente.\n");
		return -1;
	}

	if (conectarCliente(c) == -1) {
		printf("Error conectando el cliente.\n");
		return -1;
	}

#ifdef DEBUG
	imprimirCliente(c);
#endif

	enviarDatos(c, datos_enviar, MAX_DATASIZE);
	tam_recb = recibirDatos(c, datos_recb, MAX_DATASIZE);

	if (tam_recb > 0) {
		printColumns(datos_recb, tam_recb, 8);
	} else {
		printf("No recibí datos\n");
	}

	eliminarCliente(c);

	return 0;
}