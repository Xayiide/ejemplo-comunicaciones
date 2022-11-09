#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> /* rand */
#include <time.h>   /* time */

#include "inc/comun.h"

static const char abc[52] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

void printColumns(uint8_t *datos, uint64_t tam, uint8_t cols) {
    uint64_t i;
    uint8_t  c;
    for (i = 0, c = 0; i < tam; i++, c++) {
        if (c == cols) {
            c = 0;
            printf("\n");
        }
        printf("%2.2X ", datos[i]);
    }
    printf("\n");
}

void letrasAleatorias(uint8_t *buff, uint64_t tam) {
    static int once = 0;
    uint64_t i;

    if (once == 0) {
#ifdef DEBUG
        printf("Inicializando srand\n");
#endif
        srand(time(NULL));
        once = 1;
    }

    for (i = 0; i < tam; i++) {
        buff[i] = abc[rand() % 53];
    }
}

int debug(const char *format, ...) {
    int result = 5;
#ifdef DEBUG
    va_list args;
    va_start(args, format);
    result = vprintf(format, args);
    va_end(args);
#endif
    return result;
}