#ifndef LM35_H
#define LM35_H
#include "adc.h"

#ifdef __cplusplus
extern "C"
{
#endif
  #define CANT_MUESTRAS_LM35 90
  float getTempAct(void);
  float getTempMin(void);
  float getTempMax(void);
  float getTempProm(void);
  conf lm35_setup(void);

#ifdef __cplusplus
}
#endif
#endif
