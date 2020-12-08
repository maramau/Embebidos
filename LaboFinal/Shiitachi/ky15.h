#ifndef KY15_H
#define KY15_H

#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include "fnqueue.h"

const int SENSOR = 2;

#ifdef __cplusplus
extern "C"
{
#endif

  void guardarHum(void);
  void ky15_setup(void);
  uint16_t getHum(void);

#ifdef __cplusplus
}
#endif
#endif
