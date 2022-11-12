
#include "inc/simpleclient.h"
#include "inc/estados.h"
#include "../include/comun.h"
#include "../include/apero.h"

#include <stdint.h> /* uints  */
#include <stdio.h>  /* printf */
#include <stdlib.h> /* free   */


valor_ret (* estados[])(void) = {est_init, est_msaludo, est_moperac,
                                 est_rresp, est_cierra};

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
