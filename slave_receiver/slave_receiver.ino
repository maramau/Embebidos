#include <String.h>
#include <Wire.h>
#include "Arduino.h"

#define CARACTER_INICIAL '*'
#define CARACTER_SEPARADOR '/'

uint8_t tamano_mensaje;
char* tipo_mensaje_recibido;

//Retorna un puntero donde se escribio el tipo de mensaje de la respuesta
char* seleccionar_tipo_respuesta(){
  char* tipo_recibido=(char*)tipo_mensaje_recibido;
  char* toReturn;

  if(strcmp("OBTENER_TEMP",tipo_recibido) == 0){
    toReturn = "AESPONDER_TEMP";
  }else{
    if(strcmp("OBTENER_MIN",tipo_recibido) == 0){
      toReturn = "BESPONDER_MIN";
    }else{
      if(strcmp("OBTENER_MAX",tipo_recibido) == 0){
        toReturn = "CESPONDER_MAX";
      }else{
        if(strcmp("OBTENER_PROM",tipo_recibido) == 0){
          toReturn = "DESPONDER_PROM\0";
        }else{
          toReturn = "EESPONDER_TODO";
        }
      }
    }
  }
  return toReturn;
}

//Retorna un puntero donde se escribio el payload de la respuesta
char* seleccionar_payload(){
  char* tipo_recibido=(char*)tipo_mensaje_recibido;
  char* toReturn;

  if(strcmp("OBTENER_TEMP",tipo_recibido) == 0){
    toReturn = "TEMP_ACTUAL";
  }else{
    if(strcmp("OBTENER_MIN",tipo_recibido) == 0){
      toReturn = "TEMP_MIN";
    }else{
      if(strcmp("OBTENER_MAX",tipo_recibido) == 0){
        toReturn = "TEMP_MAX";
      }else{
        if(strcmp("OBTENER_PROM",tipo_recibido) == 0){
          toReturn = "TEMP_PROM\0";
        }else{
          toReturn = "TEMP_TODO";
        }
      }
    }
  }
  return toReturn;
}



// Ejecuta cada vez que recibe datos del amo
// Es un evento, se debe asociar en el setup con Wire.onReceive()
void receiveEvent(int howMany) {
  uint8_t i = 0;
  char c = NULL;

  //Tomo el tamano del mensaje
  if (Wire.available() > 3){  //Tengo 4 caracteres para leer
    c = Wire.read();  //Leo el caracter inicial
    tamano_mensaje = Wire.read(); //leo el tamano del mensaje
    c = Wire.read();  //Tomo el CARACTER_SEPARADOR
    c = Wire.read();  //Tomo el primer caracter de tipo_mensaje_recibido
  }
  //Tomo el tipo de mensaje
  while(Wire.available()>0 && c!=CARACTER_SEPARADOR){
    tipo_mensaje_recibido[i]= (char) c;
    c = (char) Wire.read();
    i++;
  }
  tipo_mensaje_recibido[i] = '\0';

  //Estoy apuntando al caracter separador
  if(Wire.available() > 0){
    c = Wire.read();  //Salteo el CARACTER_SEPARADOR y apunto al primer caracter del payload
  }

}

void requestEvent() {
  uint8_t i = 0;
  char* tipo_mensaje_respuesta;
  char* payload;

  Wire.write('*');
  Wire.write(tamano_mensaje);
  Wire.write('/');
  tipo_mensaje_respuesta = seleccionar_tipo_respuesta();
  Wire.write(tipo_mensaje_respuesta);
  Wire.write('/');
  //Aca irian los write de los floats
  payload = seleccionar_payload();
  Wire.write(payload);
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
