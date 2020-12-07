#include <stdint.h>
#include "teclado.h"
#include <avr/interrupt.h>

#define MAXMILIS 5000.0
#define MAXVALOR 1023
#define MILISPORGRADO 10.0

float currentTemp = 0;
float minTemp = 1000;
float maxTemp = -1000;
float avgTemp = 0;
float muestras[100];
uint8_t cuentaMuestras = 0;


static myCfg configSensor;


float obtenerTempActual(){
  return currentTemp;
}

float obtenerTempMin(void){
  return minTemp;
}

float obtenerTempMax(void){
  return maxTemp;
}
    
float obtenerTempPromedio(void){

  return avgTemp;
}

void promedio()
{
  int i;
  float suma = 0.0;
  for (i = 0; i < 100; i++){
    suma += muestras[i];
  }

  avgTemp = suma / 100.0;
}

void updateMinMax(float temp)
{
  if (minTemp > temp)
    minTemp = temp;

  if (maxTemp < temp)
    maxTemp = temp;
}

void obtenerTemps()
{
  uint16_t vObtenido = configSensor->valorObtenido;
  currentTemp = ((vObtenido * MAXMILIS) / MAXVALOR) / MILISPORGRADO;
  muestras[cuentaMuestras] = currentTemp;
  cuentaMuestras++;
  if (cuentaMuestras == 100){
    promedio();
    cuentaMuestras = 0;
  }

  updateMinMax(currentTemp);

}

myCfg lm35_setup(){
  configSensor = (myCfg)malloc(sizeof(struct adc_cfg));
  configSensor->canal = 1;
  configSensor->valorObtenido = 0;
  configSensor->configActiva = 1;
  configSensor->callback = obtenerTemps;

  int outputTeclado = adc_init(configSensor);

  return configSensor;
}
