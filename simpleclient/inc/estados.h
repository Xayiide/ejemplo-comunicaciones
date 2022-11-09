#ifndef _ESTADOS_H_
#define _ESTADOS_H_

/* Se usarán como índices para el puntero a funciones *estados*, así que tienen
 * que tomar valores que sean válidos como índices a ese array */
typedef enum {
    EST_INIT   = 0,
    EST_SALUDA = 1,
    EST_OPERA  = 2,
    EST_ADIOS  = 3,
    EST_CIERRA = 4,
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
    {EST_INIT,   EXITO,     EST_SALUDA},
    {EST_INIT,   FRACASO,   EST_CIERRA},
    {EST_INIT,   REINTENTA, EST_INIT},

    {EST_SALUDA, EXITO,     EST_OPERA},
    {EST_SALUDA, FRACASO,   EST_CIERRA},
    {EST_SALUDA, REINTENTA, EST_SALUDA},

    {EST_OPERA,  EXITO,     EST_ADIOS},
    {EST_OPERA,  FRACASO,   EST_CIERRA},
    {EST_OPERA,  REINTENTA, EST_OPERA},

    {EST_ADIOS,  EXITO,     EST_CIERRA},
    {EST_ADIOS,  FRACASO,   EST_CIERRA},
    {EST_ADIOS,  REINTENTA, EST_ADIOS},

    {EST_CIERRA,  EXITO,     EST_CIERRA},
    {EST_CIERRA,  FRACASO,   EST_CIERRA},
    {EST_CIERRA,  REINTENTA, EST_CIERRA},
};

#define ESTADO_INICIAL EST_INIT
#define ESTADO_FINAL   EST_CIERRA

valor_ret est_init  (void);
valor_ret est_saluda(void);
valor_ret est_opera (void);
valor_ret est_adios (void);
valor_ret est_cierra(void);

nombre_est buscar_transicion(nombre_est, valor_ret);

static valor_ret (* estados[])(void) = {est_init, est_saluda, est_opera,
                                        est_adios, est_cierra};


#endif
