#include <stdio.h>
#include "inc/estados.h"

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
