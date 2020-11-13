#include <String.h>
#include <Wire.h>
#include "Arduino.h"

#define CARACTER_INICIAL '*'
#define CARACTER_SEPARADOR '/'

uint8_t tamano_mensaje;
uint8_t tipo_mensaje[14];
float fl1 = 14.25;
uint8_t f1[4];
uint8_t f2[4];
uint8_t f3[4];
uint8_t f4[4];


// Ejecuta cada vez que recibe datos del amo
// Es un evento, se debe asociar en el setup con Wire.onReceive()
void receiveEvent(int howMany) {
  uint8_t i = 0;
  uint8_t c = NULL;

  //Tomo el tamano del mensaje
  if (Wire.available() > 3){  //Tengo 4 caracteres para leer
    c = Wire.read();  //Leo el caracter inicial
    tamano_mensaje = Wire.read(); //leo el tamano del mensaje
    c = Wire.read();  //Tomo el CARACTER_SEPARADOR
    c = Wire.read();  //Tomo el primer caracter de tipo_mensaje
  }

  //Tomo el tipo de mensaje
  while(Wire.available()>0 && c!=CARACTER_SEPARADOR){
    tipo_mensaje[i]= (char) c;
    c = Wire.read();
    i++;
  }

  //Estoy apuntando al caracter separador
  if(Wire.available() > 0){
    c = Wire.read();  //Salteo el CARACTER_SEPARADOR y apunto al primer caracter del payload
  }

  i=0;

  Serial.print(tamano_mensaje);
  Serial.print(' ');
  while(tipo_mensaje[i]!=NULL){
    Serial.print((char)tipo_mensaje[i]);
    i++;
  }
  Serial.print(' ');
  Serial.println(f1[0]);/*
  Serial.print(f2);
  Serial.print(f3);
  Serial.println(f4);*/
}

void requestEvent() {
  uint8_t i = 0;

  //Serial.println(tamano_mensaje);
  //Wire.write(tamano_mensaje); // Respuesta enviada al amo
  //Considerar meter todo en un gran buffer de uint8_t llamada mensaje y llamar solo una vez a Wire.write
  Wire.write('*');
  Wire.write(tamano_mensaje);
  Wire.write('/');
    while((i<sizeof(tipo_mensaje))){
    Wire.write(tipo_mensaje[i]);
    i++;
  }
  Wire.write('/');
  Wire.write(f1,4);/*
  Wire.write(f2);
  Wire.write(f3);
  Wire.write(f4);*/
  Wire.write('*');
}



void setup() {
  Wire.begin(8);                // Me uno al I2C con direccion 8
  Wire.onReceive(receiveEvent); // Registro funciones a eventos
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
}

void loop() {
  delay(100);
}
