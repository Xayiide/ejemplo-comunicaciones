#include <stdio.h>
#include <arpa/inet.h> /* hton   */
#include <string.h>    /* memcpy */
#include <stdint.h>    /* uints  */

#include "inc/estados.h"
#include "inc/simpleserver.h"
#include "../include/apero.h"
#include "../include/comun.h"
#include "../include/simplecmd.h"

servidor *s;
conexion *c;
uint8_t comando[MAX_DATASIZE] = {0};

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
    uint8_t datos_recb[CMD_TAM];
    uint8_t respuesta[RESP_SALUDO_LEN];

    tam_recb = recv(c->fd, (char *) datos_recb, CMD_TAM, 0);
    if (tam_recb == -1) {
        debug("Error recibiendo datos.\n");
        debug("     ESTADO RECIBE SALUDO --> (%s)\n", vr_a_str(FRACASO));
        return FRACASO;
    }

    /* FIXME: ¿Tiene sentido esto con el tamaño del recv en CMD_TAM? */
    if (tam_recb != CMD_TAM) {
        debug("El tamaño de los datos recibidos no es el esperado.\n");
        debug(" [recibidos:esperados] = [%d:%d]\n", tam_recb, CMD_TAM);
        return FRACASO;
    }
#ifdef DEBUG
    debug("datos_recb:\n");
    printColumns((uint8_t *) datos_recb, tam_recb, 8);
#endif

    if (datos_recb[0] != CMD_SALUDO) {
        debug("El mensaje recibido no es un saludo.\n");
        debug("     ESTADO RECIBE SALUDO --> (%s)\n", vr_a_str(FRACASO));
        return FRACASO;
    }

    memcpy(respuesta, RESP_SALUDO, RESP_SALUDO_LEN);
    if (enviarDatos(c, respuesta, RESP_SALUDO_LEN) == -1) {
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
    debug("datos_recb (%d):\n", tam_recb);
    printColumns((uint8_t *) datos_recb, tam_recb, 8);
#endif

    memcpy(comando, datos_recb, MAX_DATASIZE);

    debug("     ESTADO RECIBE OPERACIÓN --> (%s)\n", vr_a_str(ret));
    return ret;
}

valor_ret est_opera(void) {
    debug(" --> ESTADO OPERA\n");
    valor_ret ret = EXITO;
    uint8_t respuesta[MAX_DATASIZE];
    int datos_env;


    ret = procesa_comando(comando, MAX_DATASIZE, respuesta);

    datos_env = enviarDatos(c, respuesta, MAX_DATASIZE);
    if (datos_env == -1) {
        debug("Error enviando respuesta.\n");
        ret = REINTENTAR;
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

valor_ret procesa_comando(uint8_t* datos, uint8_t longitud, uint8_t *res) {
    valor_ret ret = EXITO;
    uint8_t cmd;

    if (longitud == 0) {
        debug("El comando tiene longitud 0.\n");
        return REINTENTAR;
    }

    cmd = datos[0];

    switch(cmd) {
    case CMD_SUMA:
        if (longitud < CMD_TAM + SUMA_TAM) {
            debug("El comando SUMA no tiene la longitud adecuada.\n");
            return FRACASO;
        } else {
            procesa_suma(datos, res);
        }
        break;
    case CMD_RESTA:
        if (longitud != 3) {
            debug("El comando RESTA no tiene la longitud adecuada.\n");
            return FRACASO;
        } else {
            procesa_resta(datos, res);
        }
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
    debug("PROCESANDO SUMA %hhu + %hhu = %hu\n", sumando1, sumando2, resultado);

    resultado = htons(resultado);
    res[0] = CMD_SUMA;
    memcpy(&res[CMD_TAM], &resultado, sizeof(uint16_t));
    printColumns(res, CMD_TAM + SUMA_TAM, 8);
 }

void procesa_resta(uint8_t *datos, uint8_t *res) {
    uint8_t  restando1 = datos[1];
    uint8_t  restando2 = datos[2];
    uint16_t resultado = 0;

    resultado = htons(restando1 > restando2 ? restando1 - restando2 : 0);
    memcpy(res, &resultado, sizeof(uint16_t));
}
