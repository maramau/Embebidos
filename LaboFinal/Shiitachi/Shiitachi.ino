#include "teclado.h"
#include "fnqueue.h"
#include "Arduino.h"
#include "critical.h"
#include "lm35.h"
#include <LiquidCrystal.h>

#define MODOACTUAL 0
#define MODOMINIMO 1
#define MODOMAXIMO 2
#define MODOPROMEDIO 3

#define MAXCOLUMNS 16
#define MAXROWS 2
#define BRILLO 200
#define LCDPIN 10

static conf configSensor;
static conf configTeclado;

uint8_t modo = MODOACTUAL;
float auxTempActual=0.0;


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);




void display(){
	switch (modo){
	  	case MODOACTUAL:
      auxTempActual = getTempAct();
	  	lcd.setCursor(0, 0);
	    lcd.print("Temp. Actual    ");
	    if(auxTempActual>60.0){
	    	lcd.setCursor(0, 1);
	    	lcd.print(String("ALERTA DE TEMP: ") + auxTempActual);	
	    }
	    else{ 
	  		lcd.setCursor(0, 1);
	    	lcd.print(String("Temp: ") + getTempAct());
		}
     	break;

	  	case MODOMINIMO:
	    lcd.setCursor(0, 0);
	    lcd.print("Temp. Min       ");
	    lcd.setCursor(0, 1);
	    lcd.print(String("Min T: ") + getTempMin());
	    break;
	  	case MODOMAXIMO:
	    lcd.setCursor(0, 0);
	    lcd.print("Temp. Max       ");
	    lcd.setCursor(0, 1);
	    lcd.print(String("Max T: ") + getTempMax());
	    break;
	  	case MODOPROMEDIO:
	    lcd.setCursor(0, 0);
	    lcd.print("Temp. Promedio ");
	    lcd.setCursor(0, 1);
	    lcd.print(String("Avg T: ") + getTempProm());
	    break;
  }
}

void up_keySelect()
{
  switch (modo)
  {
  case MODOACTUAL:
    modo = MODOMINIMO;
    break;
  case MODOMINIMO:
    modo = MODOMAXIMO;
    break;
  case MODOMAXIMO:
    modo = MODOPROMEDIO;
    break;
  case MODOPROMEDIO:
    modo = MODOACTUAL;
    break;
  }
}



void setup()
{
  configSensor = sensor_setup();
  configTeclado = teclado_setup();
  adc_setup();
  //Configurar teclado

  key_up_callback(up_keySelect, TECLA_SELECT);

  Serial.begin(9600);
  lcd.begin(MAXCOLUMNS, MAXROWS);
  analogWrite(LCDPIN, BRILLO);
  lcd.setCursor(0, 0);
  lcd.print("Temp. Actual    ");
}

void loop()
{
  //display();
  fnqueue_run();
  delay(150);
  Serial.println(getTempAct());
  Serial.println(getTempMin());
  Serial.println(getTempMax());
  Serial.println(getTempProm());
}
