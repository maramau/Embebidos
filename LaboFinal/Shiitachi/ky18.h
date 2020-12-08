#ifndef KY18_H
#define KY18_H

#include <Arduino.h>
#include "adc.h"

#ifdef __cplusplus
extern "C"
{
#endif

  void guardarLuz(void);
  conf ky18_setup(void);
  uint16_t getLuz(void);

#ifdef __cplusplus
}
#endif
#endif
