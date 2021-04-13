#include "fnqueue.h"
#include <String.h>
#include "critical.h"
#include "lm35.h"   //Sensor de temperatura
#include "ky18.h"   //Fotorresistor
#include "ky15.h"   //Sensor de humedad

//***Defino los umbrales para cada etapa del desarrollo del hongo girgola***
//MICELIO
#define TEMP_MIN_MICELIO 20
#define TEMP_MAX_MICELIO 30
#define HUM_MIN_MICELIO 20
#define HUM_MAX_MICELIO 30

//PRIMORDIO
//#define TEMP_MIN_PRIMORDIO 10
//#define TEMP_MAX_PRIMORDIO 20
//#define HUM_MIN_PRIMORDIO 80
//#define HUM_MAX_PRIMORDIO 95

//VALORES EN COMUN
#define LUM_MIN 600
#define LUM_MAX 1023

//Defino la varianza
 //Si un valor se encuentra fuera del rango optimo, 
  //con este valor puedo discernir si el valor actual es regular o malo
#define VARIANZA_TEMP 3
#define VARIANZA_HUM 5
#define VARIANZA_LUM 50

//Valores actuales
float temp_act = 0.0, hum_act = 0.0;
uint16_t lum_act = 0, last_send = 0, act_send = 0;

char estado_humedad(float hum){
  char toReturn = 'M';

  if(HUM_MIN_MICELIO-VARIANZA_HUM <= hum && hum <= HUM_MAX_MICELIO+VARIANZA_HUM){
    toReturn = 'R';
    if(HUM_MIN_MICELIO <= hum && hum <= HUM_MAX_MICELIO){
      toReturn = 'B';
    }
  }

  return toReturn;
}

char estado_luminosidad(float lum){
  char toReturn = 'M';

  if(LUM_MIN-VARIANZA_LUM <= lum && lum <= LUM_MAX+VARIANZA_LUM){
    toReturn = 'R';
    if(LUM_MIN <= lum && lum <= LUM_MAX){
      toReturn = 'B';
    }
  }

  return toReturn;
}

char estado_temperatura(float temp){
  char toReturn = 'M';

  if(TEMP_MIN_MICELIO-VARIANZA_TEMP <= temp && temp <= TEMP_MAX_MICELIO+VARIANZA_TEMP){
    toReturn = 'R';
    if(HUM_MIN_MICELIO <= temp && temp <= HUM_MAX_MICELIO){
      toReturn = 'B';
    }
  }

  return toReturn;
}


//Tomo los datos ambientales y los guardo en variables
void sensarDatos(){
  temp_act = getTempAct();
  hum_act = getHum();
  lum_act = getLuz();
}

//Tomo los datos leidos y los transmito por Serial
void enviarDatos(){
  act_send = millis();
  if(act_send - last_send >= 1000){
    last_send = act_send;
    //Imprimo por el Serial los datos segun son esperados por la App
     //Temperatura
     Serial.print(temp_act + String( "ºC|") + getTempProm() + String( "ºC|") + String(estado_temperatura(temp_act)) + String("|"));
     //Humedad
     Serial.print(hum_act + String( "%|") + getPromHum() + String( "%|") + String(estado_humedad(hum_act)) + String("|"));
     //Luminosidad
     Serial.print(lum_act + String( "|") + getPromLuz() + String( "|") + String(estado_luminosidad(lum_act))); 
  }
}

void setup(){
  lm35_setup();
  ky18_setup();
  ky15_setup();
  adc_setup();

  Serial.begin(9600);
}

void loop(){
  fnqueue_run();
  sensarDatos();
  enviarDatos();
}
