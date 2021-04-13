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
    mostrarMinimo();
    break;
  case 2:
    mostrarMaximo();
    break;
  case 3:
    mostrarPromedio();
    break;
  }
}

//Cambio el modo (actual, maximo, minimo o promedio)
void cambiarModo(){
  modo = (modo + 1) % CANT_MODOS;
}

void setup(){
  adc_setup();
  confTeclado = teclado_setup();
  confSensor = sensor_setup();

  key_up_callback(cambiarModo, BOTON_SELECT);

  Serial.begin(9600);
  lcd.begin(16, 2);
  analogWrite(10, 255); //Seteo el nivel de luz del backlight
}

void loop(){
  fnqueue_run();
  //mostrar();        //Muestro los datos en el lcd
  delay(1);
  Serial.print("ACT: ");
  Serial.println(getTempAct());
  Serial.print("MIN: ");
  Serial.println(getTempMin());
  Serial.print("MAX: ");
  Serial.println(getTempMax());
  Serial.print("PROM: ");
  Serial.println(String(getTempProm(),2));
}
