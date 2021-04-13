#ifndef LM35_H
#define LM35_H
#include <Arduino.h>
#include <string.h>
#include "adc.h"

#define TECLA_UP 1
#define TECLA_DOWN 2
#define TECLA_LEFT 3
#define TECLA_RIGHT 0
#define TECLA_SELECT 4
#define CANT_TECLAS 5

#ifdef __cplusplus
extern "C"
{
#endif
    float obtenerTempActual(void);
    float obtenerTempMin(void);
    float obtenerTempMax(void);
    float obtenerTempPromedio(void);
    myCfg lm35_setup(void);
#ifdef __cplusplus
}
#endif
#endif
