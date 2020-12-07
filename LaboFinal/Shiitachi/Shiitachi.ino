#include "teclado.h"
#include "fnqueue.h"
#include "Arduino.h"
#include "critical.h"
#include "lm35.h"
#include "ky18.h"

#define MODOACTUAL 0
#define MODOMINIMO 1
#define MODOMAXIMO 2
#define MODOPROMEDIO 3

static conf configSensor;
static conf configTeclado;
static conf configLuz;

uint8_t modo = MODOACTUAL;
float auxTempActual=0.0;

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



void setup(){
  configSensor = sensor_setup();
  configTeclado = teclado_setup();
  configLuz = ky18_setup();
  adc_setup();

  key_up_callback(up_keySelect, TECLA_SELECT);

  Serial.begin(9600);
}

void loop(){
  fnqueue_run();
  delay(500);
  Serial.print("Luz: ");
  Serial.println(getLuz());
  /*Serial.println(getTempAct());
  Serial.println(getTempMin());
  Serial.println(getTempMax());
  Serial.println(getTempProm());*/
}
