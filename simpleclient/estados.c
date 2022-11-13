#include <stdio.h>
#include <stdint.h>    /* uints  */
#include <stdlib.h>    /* free   */
#include <string.h>    /* memcpy */
#include <arpa/inet.h> /* ntohs  */

#include "inc/estados.h"
#include "inc/simpleclient.h"
#include "../include/apero.h"
#include "../include/comun.h"
#include "../include/simplecmd.h"



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


/* Variables globales */
/* Por ahora se quedan aquí pero el objetivo es que estén guardadas en una
 * estructura de estado global. Tengo que ver bien cómo hacerlo. */
cliente *c;
int op = 0;


valor_ret est_init(void) {
    debug(" --> ESTADO INICIAL\n");
    c = crearCliente(LISTENING_ADDR, LISTENING_PORT);

    if (c == NULL) {
        debug("Error creando cliente.\n");
        debug("     ESTADO INICIAL --> (%s)\n", vr_a_str(FRACASO));
        return FRACASO;
    }

    if (conectarCliente(c) == -1) {
        debug("Error conectando el cliente.\n");
        debug("     ESTADO INICIAL --> (%s)\n", vr_a_str(FRACASO));
        return FRACASO; /* REINTENTAR? */
    }

#ifdef DEBUG
    imprimirCliente(c);
#endif
    debug("     ESTADO INICIAL --> (%s)\n", vr_a_str(EXITO));
    return EXITO;
}

valor_ret est_msaludo(void) {
    debug(" --> ESTADO SALUDA\n");
    valor_ret ret = FRACASO;
    ssize_t tam_recb = 0;
    uint8_t datos_recb[MAX_DATASIZE];
    uint8_t datos_saludo[CMD_TAM] = {CMD_SALUDO};

    enviarDatos(c, datos_saludo, sizeof(datos_saludo));
    tam_recb = recibirDatos(c, datos_recb, MAX_DATASIZE);

    if (tam_recb > 0) {
        ret = EXITO;
#ifdef DEBUG
        debug("datos_recb:\n");
        printColumns(datos_recb, tam_recb, 8);
#endif
    } else {
        ret = FRACASO;
        debug("No recibí datos\n");
    }

    debug("     ESTADO SALUDA --> (%s)\n", vr_a_str(ret));
    return ret;
}

valor_ret est_moperac(void) {
    debug(" --> ESTADO OPERA\n");
    valor_ret ret = EXITO;
    uint8_t op1 = 10, op2 = 15;
    uint8_t *datos_operar;
    int datos_env;

    if (op == 0) {
        /* pedir suma */
        datos_operar = pide_suma(op1, op2);
        if (datos_operar == NULL) {
            debug("Datos nulos\n");
            return FRACASO;
        }
        datos_env = enviarDatos(c, datos_operar, CMD_TAM + SUMA_TAM);
        free(datos_operar);
        op++;
    } else if (op == 1) {
        op = 0;
    }

    if (datos_env == -1) {
        debug("Error mandando la operación\n");
        debug("     ESTADO OPERA --> (%s)\n", vr_a_str(FRACASO));
        return FRACASO;
    }

    debug("     ESTADO OPERA --> (%s)\n", vr_a_str(ret));
    return ret;
}

/* TODO: necesita saber la operación que pedimos */
valor_ret est_rresp(void) {
    debug(" --> ESTADO RECIBIR RESPUESTA\n");
    valor_ret ret = EXITO;
    ssize_t tam_recb = 0;
    uint8_t datos_recb[MAX_DATASIZE];

    tam_recb = recibirDatos(c, datos_recb, MAX_DATASIZE);

    if (tam_recb > 0) {
#ifdef DEBUG
        debug("datos_recb:\n");
        printColumns(datos_recb, 1, 8);
#endif
        ret = procesa_respuesta(datos_recb, tam_recb);
    } else {
        ret = FRACASO;
        debug("No recibí datos\n");
    }

    debug("     ESTADO RECIBIR RESPUESTA --> (%s)\n", vr_a_str(ret));
    return ret;
}

valor_ret est_cierra(void) {
    valor_ret ret = FRACASO;
    int elimret;
    debug(" --> ESTADO CIERRA\n");
    elimret = eliminarCliente(c);
    if (elimret == 0)
        ret = EXITO;
    debug("     ESTADO CIERRA --> (%s)\n", vr_a_str(ret));
    return ret;
}

nombre_est buscar_transicion(nombre_est est_act, valor_ret ret) {
    if (est_act == ESTADO_FINAL) {
        return est_act;
    }

    return transiciones[3 * (est_act) + ret].destino;
}

uint8_t *pide_suma(uint8_t sumando1, uint8_t sumando2) {
    uint8_t *peticion_op;
    size_t   nmemb = CMD_TAM + SUMA_TAM; /* 1 de comando + 2 de operandos */


    peticion_op = calloc(nmemb, sizeof(uint8_t));
    if (peticion_op == NULL) {
        printf("Error reservando memoria (calloc)\n");
        return NULL;
    }

    peticion_op[0] = CMD_SUMA;
    peticion_op[1] = sumando1;
    peticion_op[2] = sumando2;

    return peticion_op;
}

valor_ret procesa_respuesta(uint8_t *datos, uint8_t longitud) {
    valor_ret ret = EXITO;
    uint8_t cmd;

    if (longitud == 0) {
        debug("La respuesta tiene longitud 0.\n");
        return EXITO; /* volvemos a mandar operación */
    }

    cmd = datos[0];

    switch(cmd) {
    case CMD_SUMA: {
        uint16_t res = 0;
        if (longitud < CMD_TAM + sizeof(uint16_t)) { /* respuesta = uint16_t */
            debug("El comando SUMA no tiene la longitud adecuada.\n");
            return FRACASO;
        } else {
            res = procesa_suma(datos);
            debug("RESULTADO SUMA: %hu\n", res);
        }
    }
        break;
    default:
        debug("0x%02X no es un comando válido.\n", cmd);
        break;
    }

    return ret;
}

uint16_t procesa_suma(uint8_t *datos) {
    uint16_t resultado;
    uint8_t aux[2];

    memcpy(aux, &datos[1], sizeof(uint16_t));
    resultado = ntohs((uint16_t) *aux);

    return resultado;
}
