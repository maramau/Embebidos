#include "lm35.h"

float tempC = 0, minTemp = 500, maxTemp = -50, promTemp = 1, sumaMuestrasLM35 = 0;
float muestrasLM35[CANT_MUESTRAS_LM35];
uint8_t puntMuestrasLM35 = 0, cantMuestrasLM35 = 0;

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
  promTemp = sumaMuestrasLM35 / cantMuestrasLM35;
}

//Tomo la ultima medicion guardada en la configuracion del canal
  //la guardo en mi arreglo de muestrasLM35 y la proceso
void guardarTemps(){
  uint16_t ultMed = configSensor->ultMedicion;

  // Max_cant_volts * ultimo valor leido * factor de escala (la T crece 1C cada 10mV)
  // Todo dividido 1024 (la cantidad de valores posibles por el analogRead())
  tempC = (5.0 * ultMed * 100.0)/1024.0;
  sumaMuestrasLM35 = sumaMuestrasLM35 + tempC - muestrasLM35[puntMuestrasLM35];
  muestrasLM35[puntMuestrasLM35] = tempC;
  puntMuestrasLM35++;
  if(muestrasLM35[CANT_MUESTRAS_LM35-1]==0){  //Si falta poner muestrasLM35 para llenar el arreglo
    cantMuestrasLM35=puntMuestrasLM35;
  }else{                //Si el arreglo esta lleno
    cantMuestrasLM35=CANT_MUESTRAS_LM35;
  }
  if (puntMuestrasLM35 == CANT_MUESTRAS_LM35){
    puntMuestrasLM35 = 0;
  }
  setMinMaxProm(tempC);
}


//Configuracion inicial del sensor
conf lm35_setup(){
  configSensor = (conf)malloc(sizeof(struct adc_cfg));
  configSensor->canal = 0;
  configSensor->ultMedicion = 0;
  configSensor->confActual = 1;
  configSensor->callback = guardarTemps;

  adc_init(configSensor);

  return configSensor;
}
