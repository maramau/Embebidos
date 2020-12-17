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
//#define HUM_MIN_PRIMORDIO 80.0
//#define HUM_MAX_PRIMORDIO 95.0

//VALORES EN COMUN
#define LUM_MIN 80.0
#define LUM_MAX 100.0

//Defino la varianza
 //Si un valor se encuentra fuera del rango optimo, 
  //con este valor puedo discernir si el valor actual es regular o malo
#define VARIANZA_TEMP 3
#define VARIANZA_HUM 5.0
#define VARIANZA_LUM 5.0

//Valores actuales
float temp_act, hum_act, lum_act;


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


void setup(){
  lm35_setup();
  ky18_setup();
  ky15_setup();
  adc_setup();

  Serial.begin(9600);
}

void loop(){
  temp_act = getTempAct();
  hum_act = getHum();
  lum_act = getLuz();
  
  fnqueue_run();
  delay(1000);
  //Imprimo por el Serial los datos segun son esperados por la App
   //Temperatura
   Serial.print(temp_act + String( "ºC|") + getTempProm() + String( "ºC|") + String(estado_temperatura(temp_act)) + String("|"));
   //Humedad
   Serial.print(hum_act + String( "%|") + getPromHum() + String( "%|") + String(estado_humedad(hum_act)) + String("|"));
   //Luminosidad
   Serial.print(lum_act + String( "%|") + getPromLuz() + String( "%|") + String(estado_luminosidad(lum_act)));
}
