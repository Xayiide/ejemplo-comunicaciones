#include <stdio.h>  /* printf */
#include <unistd.h> /* close */

#include "inc/simpleserver.h"
#include "inc/estados.h"
#include "../include/comun.h"
#include "../include/apero.h"
#include "../include/simplecmd.h"

#define ESPERA 100000

/* TODO: Convertir estas variables globales en un struct o algo que se pueda
 * pasar de un estado a otro
 */
servidor *s;
conexion *c;
uint8_t comando[MAX_DATASIZE] = {0};


valor_ret est_init(void) {
    debug(" --> ESTADO INICIAL\n");
    s = crearServidor(LISTENING_ADDR, LISTENING_PORT);

    if (s == NULL) {
        debug("Error creando cliente.\n");
        debug("     ESTADO INICIAL --> (%s)\n", vr_a_str(FRACASO));
        return FRACASO;
    }

#ifdef DEBUG
    imprimirServidor(s);
#endif

    if (iniciarServidor(s, MAX_CONEXIONES) == -1) {
        return FRACASO;
    }

    debug("     ESTADO INICIAL --> (%s)\n", vr_a_str(EXITO));
    return EXITO;
}

/* Por ahora acepta una sola conexión */
valor_ret est_acepta(void) {
    debug(" --> ESTADO ACEPTA\n");
    valor_ret ret = EXITO;

    int                fd_x;
    struct sockaddr_in saddr_x;
    socklen_t          tam_x;

    tam_x = sizeof(saddr_x);
    fd_x  = accept(s->fd, (struct sockaddr *) &saddr_x, &tam_x);
    if (fd_x == -1) {
        debug("Error aceptando.\n");
        debug("     ESTADO ACEPTA --> (%s)\n", vr_a_str(FRACASO));
        return FRACASO;
    }
    c = crearConexion(fd_x, saddr_x);
    if (c == NULL) {
        debug("Error creando conexión.\n");
        debug("     ESTADO ACEPTA --> (%s)\n", vr_a_str(FRACASO));
        return FRACASO;
    }

#ifdef DEBUG
    imprimirConexion(c);
#endif

    debug("     ESTADO ACEPTA --> (%s)\n", vr_a_str(ret));
    return ret;
}

valor_ret est_rsaludo(void) {
    debug(" --> ESTADO RECIBE SALUDO\n");
    valor_ret ret = EXITO;
    ssize_t tam_recb;
    uint8_t datos_recb[MAX_DATASIZE];
    uint8_t respuesta[MAX_DATASIZE];

    tam_recb = recv(c->fd, (char *) datos_recb, MAX_DATASIZE, 0);
    if (tam_recb == -1) {
        debug("Error recibiendo datos.\n");
        debug("     ESTADO RECIBE SALUDO --> (%s)\n", vr_a_str(FRACASO));
        return FRACASO;
    }
#ifdef DEBUG
    debug("datos_recb:\n");
    printColumns((uint8_t *) datos_recb, 1, 8);
#endif

    if (datos_recb[0] != CMD_SALUDO) {
        debug("El mensaje recibido no es un saludo.\n");
        debug("     ESTADO RECIBE SALUDO --> (%s)\n", vr_a_str(FRACASO));
        return FRACASO;
    }

    letrasAleatorias(respuesta, MAX_DATASIZE);
    respuesta[0] = CMD_SALUDO;
    if (enviarDatos(c, respuesta, MAX_DATASIZE) == -1) {
        debug("Error enviando respuesta.\n");
        debug("     ESTADO RECIBE SALUDO --> (%s)\n", vr_a_str(FRACASO));
        return FRACASO;
    }

    debug("     ESTADO RECIBE SALUDO --> (%s)\n", vr_a_str(ret));
    return ret;
}

valor_ret est_roperac(void) {
    debug(" --> ESTADO RECIBE OPERACIÓN\n");
    valor_ret ret = EXITO;
    ssize_t tam_recb;
    uint8_t datos_recb[MAX_DATASIZE];

    tam_recb = recv(c->fd, (char *) datos_recb, MAX_DATASIZE, 0);
    if (tam_recb == 0) {
        debug("Cliente desconectado.\n");
        debug("     ESTADO RECIBE OPERACIÓN --> (%s)\n", vr_a_str(FRACASO));
        return FRACASO;
    }

    if (tam_recb == -1) {
        debug("Error recibiendo datos.\n");
        debug("     ESTADO RECIBE OPERACIÓN --> (%s)\n", vr_a_str(FRACASO));
        return FRACASO;
    }
#ifdef DEBUG
    debug("datos_recb: ");
    printColumns((uint8_t *) datos_recb, 1, 8);
#endif



    if (datos_recb[0] != 0xFF) {
        debug("Los datos recibidos no contienen un comando.\n");
        /* Volver a intentar recibir, pero quedarse bloqueado en el recv */
        /* Aquí debería estar el SELECT */
        sleep(1);
        ret = REINTENTAR;
    }
    else if (datos_recb[0] == 0xFF) {
        ret = EXITO;
    }

    /* TODO:
     * Filtrar operaciones y actuar en consecuencia
     */

    debug("     ESTADO RECIBE OPERACIÓN --> (%s)\n", vr_a_str(ret));
    return ret;
}

valor_ret est_opera(void) {
    debug(" --> ESTADO OPERA\n");
    valor_ret ret = EXITO;
    uint8_t respuesta[MAX_DATASIZE];
    int datos_env;

    static int borrar = 0;

    letrasAleatorias(respuesta, MAX_DATASIZE);
    if (borrar == 1) {
        respuesta[0] = 0xAA;
    }
    else {
        respuesta[0] = 0xFF;
    }
    datos_env = enviarDatos(c, respuesta, MAX_DATASIZE);
    if (datos_env == -1) {
        debug("Error enviando respuesta.\n");
        ret = REINTENTAR;
    }
    else {
        ret = EXITO;
        borrar++;
    }

    debug("     ESTADO OPERA --> (%s)\n", vr_a_str(ret));
    return ret;
}

valor_ret est_cierra(void) {
    debug(" --> ESTADO CIERRA\n");
    valor_ret ret = FRACASO;
    int elimret1, elimret2;

    elimret1 = eliminarConexion(c);
    elimret2 = eliminarServidor(s);
    if (elimret1 == 0 && elimret2 == 0) {
        ret = EXITO;
    }
    
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

    while (1) {
        funcion_est = estados[est_act];
        ret = funcion_est();
        if (est_act == ESTADO_FINAL) {
            return ret;
        }
        est_act = buscar_transicion(est_act, ret);
    }


    return 0;
}
