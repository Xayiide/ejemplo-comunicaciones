#ifndef _ESTADOS_H_
#define _ESTADOS_H_

/* Se usarán como índices para el puntero a funciones *estados*, así que tienen
 * que tomar valores que sean válidos como índices a ese array */
typedef enum {
    EST_INIT   ,
    EST_MSALUDO,
    EST_MOPERAC,
    EST_RRESP  ,
    EST_CIERRA ,
} nombre_est;
const char *ne_a_str(nombre_est);

typedef enum {
    EXITO     = 0,
    FRACASO   = 1,
    REINTENTA = 2,
} valor_ret;
const char *vr_a_str(valor_ret);

typedef struct {
    nombre_est origen;
    valor_ret  retorno;
    nombre_est destino;
} transicion;

static const transicion transiciones[] = {
    {EST_INIT,    EXITO,     EST_MSALUDO},
    {EST_INIT,    FRACASO,   EST_CIERRA},
    {EST_INIT,    REINTENTA, EST_INIT},

    {EST_MSALUDO, EXITO,     EST_MOPERAC},
    {EST_MSALUDO, FRACASO,   EST_CIERRA},
    {EST_MSALUDO, REINTENTA, EST_MSALUDO},

    {EST_MOPERAC, EXITO,     EST_RRESP},
    {EST_MOPERAC, FRACASO,   EST_CIERRA},
    {EST_MOPERAC, REINTENTA, EST_MOPERAC},

    {EST_RRESP,   EXITO,     EST_MOPERAC},
    {EST_RRESP,   FRACASO,   EST_CIERRA},
    {EST_RRESP,   REINTENTA, EST_RRESP},

    {EST_CIERRA,  EXITO,     EST_CIERRA},
    {EST_CIERRA,  FRACASO,   EST_CIERRA},
    {EST_CIERRA,  REINTENTA, EST_CIERRA},
};

#define ESTADO_INICIAL EST_INIT
#define ESTADO_FINAL   EST_CIERRA

valor_ret est_init   (void);
valor_ret est_msaludo(void);
valor_ret est_moperac(void);
valor_ret est_rresp  (void);
valor_ret est_cierra (void);

nombre_est buscar_transicion(nombre_est, valor_ret);
uint8_t *pide_suma (uint8_t, uint8_t);
uint8_t *pide_resta(uint8_t, uint8_t);




#endif
