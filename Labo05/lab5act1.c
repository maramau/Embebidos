#include <Arduino_FreeRTOS.h>
#include "semphr.h"
#include "task.h"
#define  LED  13

void TaskTurnOnLED( void *pvParameters );
void TaskTurnOffLED( void *pvParameters );

SemaphoreHandle_t xSemApagadoCompleto;
SemaphoreHandle_t xSemEncendidoCompleto;

void setup(){

  pinMode(LED ,OUTPUT);
  
  //Creación de semáforos binarios
  xSemApagadoCompleto = xSemaphoreCreateBinary();
  xSemEncendidoCompleto = xSemaphoreCreateBinary();

  //Creación de tareas
  xTaskCreate(ledOn, "LedON",128,NULL,1,NULL);
  xTaskCreate(ledOff, "LedOFF", 128,NULL,1,NULL);
  
  //Inicialización de semáforos
  //xSemaphoreTake(xSemEncendidoCompleto, portMAX_DELAY); //Inicializado en 0  -> Con esto no anda. :(
  xSemaphoreGive(xSemApagadoCompleto); //Inicializado en 1
}

void loop(){}

void ledOff(void *pvParameters){
while(1){
  xSemaphoreTake(xSemEncendidoCompleto, portMAX_DELAY);
  digitalWrite(LED,LOW);
  vTaskDelay(500/portTICK_PERIOD_MS);
  xSemaphoreGive(xSemApagadoCompleto);
}

  
}
void ledOn(void *pvParameters){
  while(1){
    xSemaphoreTake(xSemApagadoCompleto, portMAX_DELAY);
    digitalWrite(LED,HIGH);
    vTaskDelay(500/portTICK_PERIOD_MS);
    xSemaphoreGive(xSemEncendidoCompleto);
  }
}