#include <String.h>
#include <Wire.h>
#include "Arduino.h"
#include "String.h"
#include "critical.h"
#include "teclado.h"
#include "fnqueue.h"
#include "sensor.h"
#include "device.h"

conf confSensor;
conf confTeclado;

#define CARACTER_INI_FIN '*'
#define CARACTER_SEPARADOR '/'
#define TAMANO_FLOAT 8
#define OBTENER_TEMP '1'
#define OBTENER_MIN '2'
#define OBTENER_MAX '3'
#define OBTENER_PROM '4'
#define OBTENER_TODO '5'
#define RESPONDER_TEMP '6'
#define RESPONDER_MIN '7'
#define RESPONDER_MAX '8'
#define RESPONDER_PROM '9'
#define RESPONDER_TODO '0'
uint8_t tipo_mensaje_recibido;

//Retorna un puntero donde se escribio el tipo de mensaje de la respuesta
uint8_t seleccionar_tipo_respuesta(){
  uint8_t toReturn;

  switch (tipo_mensaje_recibido){
    case OBTENER_TEMP:
      toReturn = RESPONDER_TEMP;
    break;
    case OBTENER_MIN:
      toReturn = RESPONDER_MIN;
    break;
    case OBTENER_MAX:
      toReturn = RESPONDER_MAX;
    break;
    case OBTENER_PROM:
      toReturn = RESPONDER_PROM;
    break;
    case OBTENER_TODO:
      toReturn = RESPONDER_TODO;
    break;
  }

  return toReturn;
}

//Retorna un puntero donde se escribio el payload de la respuesta
void seleccionar_payload(char* t_act, char* t_min, char* t_max, char* t_prom){
  float f1, f2, f3, f4;
  char* toReturn;

  switch (tipo_mensaje_recibido){
    case OBTENER_TEMP:
      f1 = getTempAct();
      dtostrf(f1, 3, 2, t_act);
      break;
    case OBTENER_MIN:
      f2 = getTempMin();
      dtostrf(f2, 3, 2, t_min);
      break;
    case OBTENER_MAX:
      f3 = getTempMax();
      dtostrf(f3, 3, 2, t_max);
      break;
    case OBTENER_PROM:
      f4 = getTempProm();
      dtostrf(f4, 3, 2, t_prom);
      break;
    case OBTENER_TODO:
      f1 = getTempAct();
      dtostrf(f1, 3, 2, t_act);
      strcat(t_act, "-");
      f2 = getTempMin();
      dtostrf(f2, 3, 2, t_min);
      strcat(t_min, "-");
      f3 = getTempMax();
      dtostrf(f3, 3, 2, t_max);
      strcat(t_max, "-");
      f4 = getTempProm();
      dtostrf(f4, 3, 2, t_prom);
      break;
  }
}

// Ejecuta cada vez que recibe datos del amo
// Es un evento, se debe asociar en el setup con Wire.onReceive()
void receiveEvent(int howMany){
  char c = NULL;

  //Tomo el tamano del mensaje
  if (Wire.available() > 1){  //Tengo 2 caracteres para leer
    c = Wire.read();  //Leo el caracter inicial
    c = Wire.read();  //Tomo el primer caracter de tipo_mensaje_recibido
  }
  //Tomo el tipo de mensaje
  if(Wire.available()>0 && c!=CARACTER_SEPARADOR){
    tipo_mensaje_recibido = (uint8_t) c;
    c = (char) Wire.read();
  }

  //Estoy apuntando al caracter FINALIZADOR
    //Si queda algo mas es basura
  while(c != CARACTER_INI_FIN){
    c = Wire.read();
  }
}

void requestEvent() {
  int tamano_respuesta;
  char* payload;
  char mensaje [50];
  char t_act[TAMANO_FLOAT] = {'\0', '\0', '\0', '\0', '\0'};
  char t_min[TAMANO_FLOAT] = {'\0', '\0', '\0', '\0', '\0'};
  char t_max[TAMANO_FLOAT] = {'\0', '\0', '\0', '\0', '\0'};
  char t_prom[TAMANO_FLOAT] = {'\0', '\0', '\0', '\0', '\0'};

  mensaje[0] = CARACTER_INI_FIN;
  mensaje[1] = seleccionar_tipo_respuesta();
  mensaje[2] = '/';
  mensaje[3] = '\0';

  seleccionar_payload(t_act,t_min,t_max,t_prom);
  strcat(mensaje,t_act);
  strcat(mensaje,t_min);
  strcat(mensaje,t_max);
  strcat(mensaje,t_prom);

  strcat(mensaje,"/");
  Wire.write(mensaje);
  tamano_respuesta =(int) strlen(mensaje) + 2;
  Wire.write(tamano_respuesta);
  Wire.write(CARACTER_INI_FIN);
}

void setup() {
  adc_setup();
  confSensor = sensor_setup();
  
  tipo_mensaje_recibido = (char *) malloc(14*sizeof(char));
  Wire.begin(8);                // Me uno al I2C con direccion 8
  Wire.onReceive(receiveEvent); // Registro funciones a eventos
  Wire.onRequest(requestEvent);
}

void loop() {
  delay(50);
  fnqueue_run();
}
