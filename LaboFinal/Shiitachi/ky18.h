#ifndef KY18_H
#define KY18_H

#include "adc.h"

#ifdef __cplusplus
extern "C"
{
#endif
  #define CANT_MUESTRAS_KY18 100
  void guardarLuz(void);
  conf ky18_setup(void);
  uint16_t getLuz(void);
  float getPromLuz(void);
  
#ifdef __cplusplus
}
#endif
#endif
