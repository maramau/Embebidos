#ifndef TECLADO_H
#define TECLADO_H
#include <Arduino.h>
#include <string.h>
#include "device.h"

#define BOTON_UP 1
#define BOTON_DOWN 2
#define BOTON_LEFT 3
#define BOTON_RIGHT 0
#define BOTON_SELECT 4
#define CANT_BOTONES 5

#ifdef __cplusplus
extern "C"{
#endif

    void key_down_callback(void *handler(), int boton);
    void key_up_callback(void *handler(), int boton);
    conf teclado_setup(void);
    
#ifdef __cplusplus
}
#endif
#endif
