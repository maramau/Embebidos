#include <String.h>
#include <Wire.h>

#define TAMANO_BUFFER 80

uint8_t rx_tx_buf[TAMANO_BUFFER]; //falsibuffer creado dentro de este modulo. No escribe directamente en el bus
uint8_t tamano_mensaje;

void preProcesarEntrada(){
  //Saco los '*' y separo el tamano, el payload y el mensaje
  tamano_mensaje = (uint8_t)rx_tx_buf[1]; //Ahora se que el ultimo '*' esta en tamano_mensaje. Puedo ignorarlo si quiero

}


// Ejecuta cada vez que recibe datos del amo
// Es un evento, se debe asociar en el setup con Wire.onReceive()
void receiveEvent(int howMany) {
  int i = 0;
  while (Wire.available() > 1) {
    char c = Wire.read();
    //Serial.print(c);
    rx_tx_buf[i]=c;             //Almaceno lo leido en el buffer real en mi falsibuffer
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
  Wire.write(tamano_mensaje); // Respuesta enviada al amo
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
