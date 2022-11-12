#ifndef _ESTADOS_H_
#define _ESTADOS_H_

#include "../../include/apero.h"

/* Se usarán como índices para el puntero a funciones *estados*, así que tienen
 * que tomar valores que sean válidos como índices a ese array */
typedef enum {
    EST_INIT   ,
    EST_ACEPTA ,
    EST_RSALUDO,
    EST_ROPERAC,
    EST_OPERA,
    EST_CIERRA ,
} nombre_est;
const char *ne_a_str(nombre_est);

typedef enum {
    EXITO      = 0,
    FRACASO    = 1,
    REINTENTAR = 2,
} valor_ret;
const char *vr_a_str(valor_ret);

typedef struct {
    nombre_est origen;
    valor_ret  retorno;
    nombre_est destino;
} transicion;

static const transicion transiciones[] = {
    {EST_INIT,    EXITO,      EST_ACEPTA},
    {EST_INIT,    FRACASO,    EST_CIERRA},
    {EST_INIT,    REINTENTAR, EST_INIT},

    {EST_ACEPTA,  EXITO,      EST_RSALUDO},
    {EST_ACEPTA,  FRACASO,    EST_CIERRA},
    {EST_ACEPTA,  REINTENTAR, EST_ACEPTA},

    {EST_RSALUDO, EXITO,      EST_ROPERAC},
    {EST_RSALUDO, FRACASO,    EST_CIERRA},
    {EST_RSALUDO, REINTENTAR, EST_RSALUDO},

    {EST_ROPERAC, EXITO,      EST_OPERA},
    {EST_ROPERAC, FRACASO,    EST_CIERRA},
    {EST_ROPERAC, REINTENTAR, EST_ROPERAC},

    {EST_OPERA,   EXITO,      EST_ROPERAC},
    {EST_OPERA,   FRACASO,    EST_CIERRA},
    {EST_OPERA,   REINTENTAR, EST_OPERA},

    {EST_CIERRA,  EXITO,      EST_CIERRA},
    {EST_CIERRA,  FRACASO,    EST_CIERRA},
    {EST_CIERRA,  REINTENTAR, EST_CIERRA},
};


#define ESTADO_INICIAL EST_INIT
#define ESTADO_FINAL   EST_CIERRA

valor_ret est_init   (void);
valor_ret est_acepta (void);
valor_ret est_rsaludo(void);
valor_ret est_roperac(void);
valor_ret est_opera  (void);
valor_ret est_cierra (void);

nombre_est buscar_transicion(nombre_est, valor_ret);


valor_ret procesa_comando(uint8_t *, uint8_t, uint8_t *);
void procesa_suma (uint8_t *, uint8_t *);
void procesa_resta(uint8_t *, uint8_t *);




#endif
