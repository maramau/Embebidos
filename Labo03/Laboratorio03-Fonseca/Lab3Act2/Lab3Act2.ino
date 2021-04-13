#include "Arduino.h"
#include "String.h"
#include "critical.h"
#include "teclado.h"
#include "fnqueue.h"
#include "sensor.h"
#include "device.h"
#include <LiquidCrystal.h>

#define UMBRAL 60.0
#define CANT_MODOS 4

conf confSensor;
conf confTeclado;
unsigned long lastSend = 0;
unsigned long lastActual = 0;
uint8_t modo = 0;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Muestro la temperatura actual
void mostrarActual(){
  unsigned long tiempo = millis(); //El tiempo que paso desde que el Arduino empezo a correr
                                   //Si paso 1 seg desde el ultimo mostrar imprimo
                                   //Esto para evitar las continuas y molestas actualizaciones del LCD
  if (tiempo - lastActual > 1000){
    lastActual = tiempo;
    float tempAct = getTempAct();
    if (tempAct > UMBRAL){
      lcd.setCursor(0, 0);
      lcd.print("     PELIGRO    ");
      lcd.setCursor(0, 1);
      lcd.print("UMBRAL1 SUPERADO");
    }
    else{
      lcd.setCursor(0, 0);
      lcd.print("    T Actual    ");
      lcd.setCursor(0, 1);
      lcd.print(String("      ") + tempAct + String("     "));
    }
  }
}

//Muestro la temperatura minima
void mostrarMinimo(){
  lcd.setCursor(0, 0);
  lcd.print("    T Minimo    ");
  lcd.setCursor(0, 1);
  lcd.print(String("      ") + getTempMin() + String("     "));
}

//Muestro la temperatura maximo
void mostrarMaximo(){
  lcd.setCursor(0, 0);
  lcd.print("    T Maximo    ");
  lcd.setCursor(0, 1);
  lcd.print(String("      ") + getTempMax() + String("     "));
}

//Muestro la temperatura promedio
void mostrarPromedio(){
  lcd.setCursor(0, 0);
  lcd.print("   T Promedio   ");
  lcd.setCursor(0, 1);
  lcd.print(String("      ") + getTempProm() + String("     "));
}


//Segun el modo muestro alguna de los valores de temperaturas
void mostrar(){
  switch (modo){
  case 0:
    mostrarActual();
    break;
  case 1:
     mostrarMaximo();
    break;
  case 2:
    mostrarMinimo();
    break;
  case 3:
    mostrarPromedio();
    break;
  }
}

//Envio los valores de las temperaturas actual, maxima, minima y promedio al serial
  //para comunicarse con el processing
void enviarMuestras(){
  unsigned long tiempo = millis();
  
  if (tiempo - lastSend > 500){
    lastSend = tiempo;
    String tempAct = String(getTempAct(), 2);
    String tempMin = String(getTempMin(), 2);
    String tempMax = String(getTempMax(), 2);
    String tempProm = String(getTempProm(), 2);
    String temps = String("(" + tempAct + "/" + tempMin + "/" + tempMax + "/" + tempProm + "/" + modo + ")");
    Serial.println(getTempMin());
  }
}

//Cambio el modo (actual, maximo, minimo o promedio)
void cambiarModo(){
  modo = (modo + 1) % CANT_MODOS;
}

//Leo el serial para ver si me llego una orden del processing por si debo cambiar de modo
void recibirModo(){
  char estado;
  
  if (Serial.available()){
    estado = Serial.read();
  }
  if (estado == 'X'){
    cambiarModo();
  }
}

void setup(){
  adc_setup();
  confTeclado = teclado_setup();
  confSensor = sensor_setup();

  key_up_callback(cambiarModo, BOTON_SELECT);

  Serial.begin(9600);
  //lcd.begin(16, 2);
  //analogWrite(10, 255); //Seteo el nivel de luz del backlight
}

void loop(){
  Serial.print("LOOP");
  delay(500);
  fnqueue_run();
  //mostrar();        //Muestro los datos en el lcd
  recibirModo();    //Recibo el cambio de modo de la GUI
  enviarMuestras(); //Tomo del sensor y mando las muestras a la GUI
  
}
