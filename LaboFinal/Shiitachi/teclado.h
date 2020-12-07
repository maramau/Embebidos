#ifndef TECLADO_H
#define TECLADO_H
#include <Arduino.h>
#include <string.h>
#include "adc.h"

#define TECLA_UP 1
#define TECLA_DOWN 2
#define TECLA_LEFT 3
#define TECLA_RIGHT 0
#define TECLA_SELECT 4
#define CANT_TECLAS 5

struct structMyKey
{
  void (*callback_keyUp)();
  void (*callback_keyDown)();
} typedef myKey;

#ifdef __cplusplus
extern "C"
{
#endif
    void key_down_callback(void *handler(), int tecla);
    void key_up_callback(void *handler(), int tecla);
    conf teclado_setup(void);
#ifdef __cplusplus
}
#endif
#endif
