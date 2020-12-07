#include "ky18.h"
#include <avr/interrupt.h>

static conf configSensor;
float muestrasKY18[100];
uint8_t puntMuestrasKY18 = 0, cantMuestrasKY18 = 0;

void guardarLuz(){
  uint16_t ultMed = configSensor->ultMedicion;

  muestrasKY18[puntMuestrasKY18++] = ultMed;
  if(muestrasKY18[99]==0){  //Si falta poner muestrasKY18 para llenar el arreglo
    cantMuestrasKY18 = puntMuestrasKY18;
  }else{                //Si el arreglo esta lleno
    cantMuestrasKY18 = 100;
  }
  if (puntMuestrasKY18 == 100){
    puntMuestrasKY18 = 0;
  }
}

uint16_t getLuz(){
  return muestrasKY18[puntMuestrasKY18-1];
}


//Configuracion inicial del sensor
conf ky18_setup(){
  configSensor = (conf)malloc(sizeof(struct adc_cfg));
  configSensor->canal = 1;
  configSensor->ultMedicion = 0;
  configSensor->confActual = 0;
  configSensor->callback = guardarLuz;

  adc_init(configSensor);

  return configSensor;
}
