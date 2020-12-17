#ifndef KY18_H
#define KY18_H

#include "adc.h"

#ifdef __cplusplus
extern "C"
{
#endif
  #define CANT_MUESTRAS_KY18 90
  void guardarLuz(void);
  conf ky18_setup(void);
  float getLuz(void);
  float getPromLuz(void);
  
#ifdef __cplusplus
}
#endif
#endif
