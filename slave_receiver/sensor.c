#include <stdint.h>
#include "teclado.h"
#include <avr/interrupt.h>

float tempC = 0, minTemp = 99.99, maxTemp = 0.00, promTemp = 1, sumaMuestras = 0;
float muestras[100];
uint8_t puntMuestras = 0, cantMuestras = 0;

static conf configSensor;

//Obtengo el valor de la temperatura actual
float getTempAct(){
  return tempC;
}

//Obtengo el valor de la temperatura maxima
float getTempMax(){
  return maxTemp;
}

//Obtengo el valor de la temperatura minima
float getTempMin(){
  return minTemp;
}

//Obtengo el valor de la temperatura promedio
float getTempProm(){
  return promTemp;
}

//Seteo los valores de la temperaturas maxima, minima y promedio
void setMinMaxProm(float ultTemp){
  if (minTemp > ultTemp){
    minTemp = ultTemp;
  }
  if (maxTemp < ultTemp){
    maxTemp = ultTemp;
  }
  promTemp = sumaMuestras / cantMuestras;
}

//Tomo la ultima medicion guardada en la configuracion del canal
  //la guardo en mi arreglo de muestras y la proceso
void guardarTemps(){
  uint16_t ultMed = configSensor->ultMedicion;
  
  // Max_cant_volts * ultimo valor leido * factor de escala (la T crece 1C cada 10mV) 
  // Todo dividido 1024 (la cantidad de valores posibles por el analogRead())
  tempC = (5.0 * ultMed * 100.0)/1024.0;
  sumaMuestras = sumaMuestras + tempC - muestras[puntMuestras];
  muestras[puntMuestras] = tempC;
  puntMuestras++;
  if(muestras[99]==0){  //Si falta poner muestras para llenar el arreglo
    cantMuestras=puntMuestras;
  }else{                //Si el arreglo esta lleno
    cantMuestras=100;
  }
  if (puntMuestras == 100){
    puntMuestras = 0;
  }
  setMinMaxProm(tempC);
}


//Configuracion inicial del sensor
conf sensor_setup(){
  configSensor = (conf)malloc(sizeof(struct adc_cfg));  //El LCD no se prende por alguna razon si no lo hago
                                                        //Se estara reescribiendo la memoria dinamica de la estructura?
  configSensor->canal = 1;
  configSensor->ultMedicion = 0;
  configSensor->confActual = 1;
  configSensor->callback = guardarTemps;

  adc_init(configSensor);

  return configSensor;
}
