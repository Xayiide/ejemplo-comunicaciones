#include <stdio.h>
#include "inc/estados.h"

static const char *estados_str[] = {
    "ESTADO INICIAL",
    "ESTADO MANDAR SALUDO",
    "ESTADO MANDAR OPERACIÓN",
    "ESTADO RECIBIR ADIÓS",
    "ESTADO CIERRA",
};

static const char *retorno_str[] = {
    "ÉXITO",
    "FRACASO",
    "REINTENTA",
};


const char *ne_a_str(nombre_est ne) {
    return (const char *) estados_str[ne];
}

const char *vr_a_str(valor_ret vr) {
    return (const char *) retorno_str[vr];
}

