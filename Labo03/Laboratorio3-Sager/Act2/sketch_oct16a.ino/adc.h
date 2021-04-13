#ifndef ADC_C
#define ADC_C
#include "Arduino.h"

#define CANALES 2

typedef struct adc_cfg
{
	uint8_t canal;
	uint8_t configActiva;
	uint16_t valorObtenido;
	void (*callback)();
} * myCfg;

#ifdef __cplusplus
extern "C"
{
#endif
	int adc_init(myCfg cfg);
	int adc_setup(void);
	uint16_t valorSensor(void);
	
#ifdef __cplusplus
}
#endif
#endif
