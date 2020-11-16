#include <String.h>
#include <Wire.h>
#include "Arduino.h"

#define CARACTER_INICIAL '*'
#define CARACTER_SEPARADOR '/'
#define TAMANO_FLOAT 8

char* tipo_mensaje_recibido;

//Retorna un puntero donde se escribio el tipo de mensaje de la respuesta
char* seleccionar_tipo_respuesta(){
  char* tipo_recibido=(char*)tipo_mensaje_recibido;
  char* toReturn;

  if(strcmp("OBTENER_TEMP",tipo_recibido) == 0){
    toReturn = "RESPONDER_TEMP";
  }else{
    if(strcmp("OBTENER_MIN",tipo_recibido) == 0){
      toReturn = "RESPONDER_MIN";
    }else{
      if(strcmp("OBTENER_MAX",tipo_recibido) == 0){
        toReturn = "RESPONDER_MAX";
      }else{
        if(strcmp("OBTENER_PROM",tipo_recibido) == 0){
          toReturn = "RESPONDER_PROM";
        }else{
          toReturn = "RESPONDER_TODO";
        }
      }
    }
  }
  return toReturn;
}

//Retorna un puntero donde se escribio el payload de la respuesta
void seleccionar_payload(char* t_act, char* t_min, char* t_max, char* t_prom){
  char* tipo_recibido=(char*)tipo_mensaje_recibido;
  float f1 = 1.11, f2 = 2.22, f3 = 3.33, f4 = 4.44;

  if(strcmp("OBTENER_TEMP",tipo_recibido) == 0){
    //f1 es el float a adaptar, 3 is mininum width, 2 is precision; float value is copied onto buff
    dtostrf(f1, 3, 2, t_act);
  }else{
    if(strcmp("OBTENER_MIN",tipo_recibido) == 0){
      dtostrf(f2, 3, 2, t_min);
    }else{
      if(strcmp("OBTENER_MAX",tipo_recibido) == 0){
        dtostrf(f3, 3, 2, t_max);
      }else{
        if(strcmp("OBTENER_PROM",tipo_recibido) == 0){
          dtostrf(f4, 3, 2, t_prom);
        }else{
          dtostrf(f1, 3, 2, t_act);
          dtostrf(f2, 3, 2, t_min);
          dtostrf(f3, 3, 2, t_max);
          dtostrf(f4, 3, 2, t_prom);
        }
      }
    }
  }
}



// Ejecuta cada vez que recibe datos del amo
// Es un evento, se debe asociar en el setup con Wire.onReceive()
void receiveEvent(int howMany) {
  uint8_t i = 0;
  char c = NULL;

  //Tomo el tamano del mensaje
  if (Wire.available() > 1){  //Tengo 2 caracteres para leer
    c = Wire.read();  //Leo el caracter inicial
    c = Wire.read();  //Tomo el primer caracter de tipo_mensaje_recibido
  }
  //Tomo el tipo de mensaje
  while(Wire.available()>0 && c!=CARACTER_SEPARADOR){
    tipo_mensaje_recibido[i]= (char) c;
    c = (char) Wire.read();
    i++;
  }
  tipo_mensaje_recibido[i] = '\0';

  //Estoy apuntando al caracter FINALIZADOR
    //Si queda algo mas es basura
  if(Wire.available() > 0){
    c = Wire.read();  //Salteo el CARACTER_SEPARADOR y apunto al primer caracter del payload
  }
}

void requestEvent() {
  int tamano_respuesta;
  char* tipo_mensaje_respuesta, * payload;
  char mensaje [50];
  char t_act[TAMANO_FLOAT];
  char t_min[TAMANO_FLOAT];
  char t_max[TAMANO_FLOAT];
  char t_prom[TAMANO_FLOAT];



  tipo_mensaje_respuesta = seleccionar_tipo_respuesta();

  mensaje[0] = '*';
  mensaje[1] = '\0';
  strcat(mensaje,(char*)tipo_mensaje_respuesta);
  strcat(mensaje,"/");


  seleccionar_payload(t_act,t_min,t_max,t_prom);
  strcat(mensaje,t_act);
  strcat(mensaje,"-\0");

  strcat(mensaje,t_min);


  strcat(mensaje,"-\0");
  strcat(mensaje,t_max);

  //strcat(mensaje,"-\0");
  //strcat(mensaje,t_prom);

  strcat(mensaje,"/");
  Wire.write(mensaje);

  tamano_respuesta =(int) strlen(mensaje) + 2;
  Wire.write(tamano_respuesta);
  Wire.write("*\0");
}



void setup() {
  tipo_mensaje_recibido = (char *) malloc(13*sizeof(char));
  Wire.begin(8);                // Me uno al I2C con direccion 8
  Wire.onReceive(receiveEvent); // Registro funciones a eventos
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
}

void loop() {
  delay(100);
}
