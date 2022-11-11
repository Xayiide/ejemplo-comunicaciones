#include <stdio.h>
#include "inc/estados.h"
#include "inc/simpleserver.h"

static const char *estados_str[] = {
    "ESTADO INICIAL",
    "ESTADO ACEPTA",
    "ESTADO RECIBIR SALUDO",
    "ESTADO RECIBIR OPERACIÓN",
    "ESTADO OPERA",
    "ESTADO CIERRA",
};

static const char *retorno_str[] = {
    "ÉXITO",
    "FRACASO",
    "REINTENTAR",
};

const char *ne_a_str(nombre_est ne) {
    return (const char *) estados_str[ne];
}

const char *vr_a_str(valor_ret vr) {
    return (const char *) retorno_str[vr];
}

uint8_t *procesa_comando(uint8_t* datos, uint8_t longitud) {
    uint8_t ret[MAX_DATASIZE];
    uint8_t cmd;

    if (longitud == 0) {
        debug("El comando tiene longitud 0.\n");
        return NULL;
    }

    cmd = datos[0];

    switch(cmd) {
    case
    }

    return ret;
}
