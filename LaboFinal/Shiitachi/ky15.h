#ifndef KY15_H
#define KY15_H

#include <DHT.h>
#include "fnqueue.h"

const int SENSOR = 2;
const int CANT_MUESTRAS_KY15 = 100;
#ifdef __cplusplus
extern "C"
{
#endif
  void guardarHum(void);
  void ky15_setup(void);
  uint16_t getHum(void);
  float getPromHum(void);

#ifdef __cplusplus
}
#endif
#endif
