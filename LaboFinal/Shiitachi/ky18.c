#include "ky18.h"

static conf configSensor;
float muestrasKY18[CANT_MUESTRAS_KY18], promLuz = 1.0, sumaMuestrasKY18 = 0;
uint8_t puntMuestrasKY18 = 0, cantMuestrasKY18 = 0;


void setPromLuz(){
  promLuz = sumaMuestrasKY18 / cantMuestrasKY18;
}

uint16_t getLuz(){
  return muestrasKY18[puntMuestrasKY18-1];
}

uint16_t getPromLuz(){
  return promLuz;
}

void guardarLuz(){
  //Muestro la luminosidad como un porcentaje
   //Luminosidad = Valor medido * 100 / Valores posibles
  uint16_t ultMed = configSensor->ultMedicion;

  sumaMuestrasKY18 = sumaMuestrasKY18 + ultMed - muestrasKY18[puntMuestrasKY18];
  muestrasKY18[puntMuestrasKY18++] = ultMed;
  if(muestrasKY18[CANT_MUESTRAS_KY18-1]==0){  //Si falta poner muestrasKY18 para llenar el arreglo
    cantMuestrasKY18 = puntMuestrasKY18;
  }else{               
    cantMuestrasKY18 = CANT_MUESTRAS_KY18;
  }
  if (puntMuestrasKY18 == CANT_MUESTRAS_KY18){
    puntMuestrasKY18 = 0;
  }

  setPromLuz();
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
