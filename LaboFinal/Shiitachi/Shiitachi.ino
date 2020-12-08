#include "fnqueue.h"
#include "Arduino.h"
#include "critical.h"
#include "lm35.h"   //Sensor de temperatura
#include "ky18.h"   //Fotorresistor
#include "ky15.h"   //Sensor de humedad

static conf configSensor;
static conf configLuz;

void setup(){
  configSensor = sensor_setup();
  configLuz = ky18_setup();
  ky15_setup();
  adc_setup();

  Serial.begin(9600);
}

void loop(){
  fnqueue_run();
  delay(1000);
  Serial.print("Luz: ");
  Serial.println(getLuz());
  Serial.print("Temperatura: ");
  Serial.println(getTempAct());
  Serial.print("Humedad: ");
  Serial.println(getHum());
  /*
  Serial.println(getTempMin());
  Serial.println(getTempMax());
  Serial.println(getTempProm());*/
}
