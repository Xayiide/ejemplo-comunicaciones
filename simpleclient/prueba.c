
#include "../include/comun.h"
#include "../include/apero.h"
#include "inc/simpleclient.h"
#include "inc/estados.h"

#include <stdint.h> /* uint8_t */
#include <stdio.h>  /* printf */

uint8_t datos_saludo[MAX_DATASIZE] = {
    0x41, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
};

uint8_t datos_operar[MAX_DATASIZE] = {
    0xFF, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
};

uint8_t datos_adios[MAX_DATASIZE] = {
    0xAA, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
};

/* Variables globales */
/* Por ahora se quedan aquí pero el objetivo es que estén guardadas en una
 * estructura de estado global. Tengo que ver bien cómo hacerlo. */
cliente *c;


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

    enviarDatos(c, datos_saludo, MAX_DATASIZE);
    tam_recb = recibirDatos(c, datos_recb, MAX_DATASIZE);

    if (tam_recb > 0) {
        ret = EXITO;
#ifdef DEBUG
        debug("datos_recb:\n");
        printColumns(datos_recb, 1, 8);
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
    int datos_env;

    datos_env = enviarDatos(c, datos_operar, MAX_DATASIZE);
    if (datos_env == -1) {
        debug("Error mandando la operación\n");
        debug("     ESTADO OPERA --> (%s)\n", vr_a_str(FRACASO));
        return FRACASO;
    }

    debug("     ESTADO OPERA --> (%s)\n", vr_a_str(ret));
    return ret;
}

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
        if (datos_recb[0] == 0xFF) {
            ret = EXITO;
        }
        else {
            ret = FRACASO;
        }
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




int main(void) {
    nombre_est est_act = ESTADO_INICIAL;
    valor_ret  ret;

    valor_ret (* funcion_est)(void);

    while(1) {
        funcion_est = estados[est_act];
        ret = funcion_est();
        if (est_act == ESTADO_FINAL) {
            return ret;
        }
        est_act = buscar_transicion(est_act, ret);
    }

    return 0;
}
