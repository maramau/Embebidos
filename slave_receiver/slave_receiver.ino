// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.
#include <String.h>
#include <Wire.h>

#define TAMANO_BUFFER 80

uint8_t rx_tx_buf[TAMANO_BUFFER]; //Buffer creado dentro de este modulo. No escribe directamente en el bus
uint8_t tamano_mensaje;

void preProcesarEntrada(){
  //Saco los '*' y separo el tamano, el payload y el mensaje
  tamano_mensaje = (uint8_t)rx_tx_buf[1]; //Ahora se que el ultimo '*' esta en tamano_mensaje. Puedo ignorarlo si quiero

}


// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  int i = 0;
  while (Wire.available() > 1) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    //Serial.print(c);         // print the character
    rx_tx_buf[i]=c;
    i++;
  }

  while(i<TAMANO_BUFFER){
    rx_tx_buf[i] = '/0';
    i++;
  }

  Serial.println((char *)rx_tx_buf);
}

void requestEvent() {
  preProcesarEntrada();
  Serial.println(tamano_mensaje);
  Wire.write(tamano_mensaje); // respond with message of 6 bytes
  // as expected by master
}



void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop() {
  delay(100);
}
