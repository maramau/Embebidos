#ifndef DEVICE_C
#define DEVICE_C

#include "Arduino.h"

//Estructura que contiene 
  //El numero del canal a ser usado por esta configuracion
  //El numero de la configuracion actual usada
  //La ultima medicion realizada en ese canal
  //La funcion de callback a ser llamada en el canal
typedef struct adc_cfg{
	uint8_t canal;
	uint8_t confActual;
	uint16_t ultMedicion;
	void (*callback)();
} * conf;

#ifdef __cplusplus
extern "C"
{
#endif
	int adc_init(conf cfg);
	int adc_setup(void);
	uint16_t valorSensor(void);
	
#ifdef __cplusplus
}
#endif
#endif
