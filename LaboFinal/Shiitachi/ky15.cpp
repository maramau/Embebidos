#include "ky15.h"

float muestrasKY15[CANT_MUESTRAS_KY15], promHum = 1.0, sumaMuestrasKY15 = 0;;
uint8_t puntMuestrasKY15 = 0, cantMuestrasKY15 = 0;
uint16_t millisPrev = 0, millisAct = 0;

DHT dht(SENSOR,DHT11);

float getPromHum(){
  return promHum;
}

uint16_t getHum(){
  return muestrasKY15[puntMuestrasKY15-1];
}

void setPromHum(){
  promHum = sumaMuestrasKY15 / cantMuestrasKY15;
}

void guardarHum(){
  uint8_t ultMed = dht.readHumidity();

  sumaMuestrasKY15 = sumaMuestrasKY15 + ultMed - muestrasKY15[puntMuestrasKY15];
  muestrasKY15[puntMuestrasKY15++] = ultMed;
  if(muestrasKY15[CANT_MUESTRAS_KY15-1]==0){  //Si falta poner muestrasKY15 para llenar el arreglo
    cantMuestrasKY15 = puntMuestrasKY15;
  }else{                    //Si el arreglo esta lleno
    cantMuestrasKY15 = CANT_MUESTRAS_KY15;
  }
  if (puntMuestrasKY15 == CANT_MUESTRAS_KY15){
    puntMuestrasKY15 = 0;
  }

  setPromHum();
}

void ky15_loop(){
  millisAct = millis();
  if((millisAct - millisPrev) > 500){
    millisPrev = millisAct;
    guardarHum();
  }
  fnqueue_add(ky15_loop);
}

//Configuracion inicial del sensor
void ky15_setup(){
  fnqueue_add(ky15_loop);
  dht.begin();
}
