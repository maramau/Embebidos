#include "fnqueue.h"
#include "Arduino.h"
#include "critical.h"
#include "lm35.h"
#include "ky18.h"

static conf configSensor;
static conf configLuz;

void setup(){
  configSensor = sensor_setup();
  configLuz = ky18_setup();
  adc_setup();

  Serial.begin(9600);
}

void loop(){
  fnqueue_run();
  delay(500);
  Serial.print("Luz: ");
  Serial.println(getLuz());
  Serial.print("Temperatura: ");
  Serial.println(getTempAct());/*
  Serial.println(getTempMin());
  Serial.println(getTempMax());
  Serial.println(getTempProm());*/
}
