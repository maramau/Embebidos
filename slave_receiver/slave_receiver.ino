#include <String.h>
#include <Wire.h>
#include "Arduino.h"

#define CARACTER_INICIAL '*'
#define CARACTER_SEPARADOR '/'
#define TAMANO_FLOAT 8
#define OBTENER_TEMP 1
#define OBTENER_MIN 2
#define OBTENER_MAX 3
#define OBTENER_PROM 4
#define OBTENER_TODO 5
#define RESPONDER_TEMP 6
#define RESPONDER_MIN 7
#define RESPONDER_MAX 8
#define RESPONDER_PROM 9
#define RESPONDER_TODO 10
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
  float f1 = 1.11, f2 = 2.22, f3 = 3.33, f4 = 4.44;

  switch (tipo_mensaje_recibido){
    case OBTENER_TEMP:
      dtostrf(f1, 3, 2, t_act);
    break;
    case OBTENER_MIN:
      dtostrf(f2, 3, 2, t_min);
    break;
    case OBTENER_MAX:
      dtostrf(f3, 3, 2, t_max);
    break;
    case OBTENER_PROM:
      dtostrf(f4, 3, 2, t_prom);
    break;
    case OBTENER_TODO:
      dtostrf(f1, 3, 2, t_act);
      //strcat(t_act, "-\0");
      dtostrf(f2, 3, 2, t_min);
      //strcat(t_min, "-\0");
      dtostrf(f3, 3, 2, t_max);
      //strcat(t_max, "-\0");
      dtostrf(f4, 3, 2, t_prom);
    break;
  }
}


// Ejecuta cada vez que recibe datos del amo
// Es un evento, se debe asociar en el setup con Wire.onReceive()
void receiveEvent(int howMany){
  uint8_t i = 0;
  char c = NULL;
 
  //Tomo el tamano del mensaje
  if (Wire.available() > 1){  //Tengo 2 caracteres para leer
    c = Wire.read();  //Leo el caracter inicial
    c = Wire.read();  //Tomo el primer caracter de tipo_mensaje_recibido
  }
  //Tomo el tipo de mensaje
  Serial.println("ENTRE");
  if(Wire.available()>0 && c!=CARACTER_SEPARADOR){
    tipo_mensaje_recibido = (uint8_t) c;
    c = (char) Wire.read();
    i++;
  }

  //Estoy apuntando al caracter FINALIZADOR
    //Si queda algo mas es basura
  while(c != CARACTER_INICIAL){
    c = Wire.read(); 
  }
}

void requestEvent() {
  int tamano_respuesta;
  char* payload;
  char mensaje [50];
  char t_act[TAMANO_FLOAT];
  char t_min[TAMANO_FLOAT];
  char t_max[TAMANO_FLOAT];
  char t_prom[TAMANO_FLOAT];
  
  mensaje[0] = '*';
  mensaje[1] = seleccionar_tipo_respuesta();
  mensaje[2] = '/';
  mensaje[3] = '\0';

  seleccionar_payload(t_act,t_min,t_max,t_prom);
  strcat(mensaje,t_act);
  Serial.println(t_act);
  strcat(mensaje,"-");
  strcat(mensaje,t_min);
  strcat(mensaje,"-");
  strcat(mensaje,t_max);
  strcat(mensaje,"-");
  strcat(mensaje,t_prom);

  strcat(mensaje,"/");
  Wire.write(mensaje);
  tamano_respuesta =(int) strlen(mensaje) + 2;
  Wire.write(tamano_respuesta);
  Wire.write("@");
  Wire.write('\0');
  Serial.println(mensaje);
}



void setup() {
  tipo_mensaje_recibido = (char *) malloc(14*sizeof(char));
  Wire.begin(8);                // Me uno al I2C con direccion 8
  Wire.onReceive(receiveEvent); // Registro funciones a eventos
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
}

void loop() {
  delay(100);
}
