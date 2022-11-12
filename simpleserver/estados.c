#include <stdio.h>
#include <arpa/inet.h> /* hton */

#include "inc/estados.h"
#include "inc/simpleserver.h"
#include "../include/simplecmd.h"

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

uint8_t *procesa_comando(uint8_t* datos, uint8_t longitud, uint8_t *res) {
    valor_ret ret = EXITO;
    uint8_t cmd;

    if (longitud == 0) {
        debug("El comando tiene longitud 0.\n");
        return REINTENTAR;
    }

    cmd = datos[0];

    switch(cmd) {
    case CMD_SUMA:
        if (longitud != 3) {
            debug("El comando SUMA no tiene la longitud adecuada.\n");
            return NULL;
        } else {
            procesa_suma(datos, res);
        }
        break;
    case CMD_RESTA:
        break;
    case CMD_MULT:
        break;
    case CMD_NONE:
        break;
    default:
        debug("0x%02X no es un comando válido.\n", cmd);
        ret = REINTENTAR;
        break;
    }

    return ret;
}

void procesa_suma(uint8_t* datos, uint8_t *res) {
    uint8_t  sumando1  = datos[1];
    uint8_t  sumando2  = datos[2];
    uint16_t resultado = sumando1 + sumando2;

    resultado = htons(resultado);
    memcpy(res, resultado, sizeof(uint16_t));
}
