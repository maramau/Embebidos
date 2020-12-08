#include "ky15.h"

float muestrasKY15[100];
uint8_t puntMuestrasKY15 = 0, cantMuestrasKY15 = 0;
uint16_t millisPrev = 0, millisAct = 0;

DHT dht(SENSOR,DHT11);

void guardarHum(){
  uint8_t ultMed = dht.readHumidity();

  muestrasKY15[puntMuestrasKY15++] = ultMed;
  if(muestrasKY15[99]==0){  //Si falta poner muestrasKY15 para llenar el arreglo
    cantMuestrasKY15 = puntMuestrasKY15;
  }else{                    //Si el arreglo esta lleno
    cantMuestrasKY15 = 100;
  }
  if (puntMuestrasKY15 == 100){
    puntMuestrasKY15 = 0;
  }
}

uint16_t getHum(){
  return muestrasKY15[puntMuestrasKY15-1];
}

void ky15_loop(){
  millisAct = millis();
  //guardarHum();
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
