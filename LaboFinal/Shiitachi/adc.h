#ifndef ADC_C
#define ADC_C
#include "Arduino.h"

#define CANT_CANALES 3

typedef struct adc_cfg
{
	uint8_t canal;          //Numero del canal que va a tomar esta configuracion
	uint8_t confActual;     //Flag que indica si esta actualmente activo o no
	uint16_t ultMedicion;   //Ultima medicion registrada
	void (*callback)();
}*conf;

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
