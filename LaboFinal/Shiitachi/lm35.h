#ifndef LM35_H
#define LM35_H
#include <Arduino.h>
#include <string.h>
#include "device.h"

#ifdef __cplusplus
extern "C"
{
#endif

  float getTempAct(void);
  float getTempMin(void);
  float getTempMax(void);
  float getTempProm(void);
  conf sensor_setup(void);

#ifdef __cplusplus
}
#endif
#endif
