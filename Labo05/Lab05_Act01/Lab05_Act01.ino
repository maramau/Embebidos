#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define FRECUENCIA 1000
#define LED 13

SemaphoreHandle_t semBinOff;
SemaphoreHandle_t semBinOn;

// define two tasks for Blink
void TaskBlinkOn(void *pvParameters);
void TaskBlinkOff(void *pvParameters);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }
  
  semBinOff = xSemaphoreCreateBinary();
  semBinOn = xSemaphoreCreateBinary();
  xSemaphoreGive(semBinOff);
  
  pinMode(LED, OUTPUT);

  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskBlinkOn
    ,  "BlinkOn"
    ,  128
    ,  NULL
    ,  2
    ,  NULL );

  xTaskCreate(
    TaskBlinkOff
    ,  "BlinkOff"
    ,  128
    ,  NULL
    ,  1
    ,  NULL );

  vTaskStartScheduler();
}

void loop(){
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlinkOn(void *pvParameters){
  (void) pvParameters;
  
  while(1){
    xSemaphoreTake(semBinOn, portMAX_DELAY);
    digitalWrite(LED, HIGH);
    vTaskDelay(FRECUENCIA / portTICK_PERIOD_MS );
    xSemaphoreGive(semBinOff);
  }
}

void TaskBlinkOff(void *pvParameters){
  (void) pvParameters;

  while(1){
    xSemaphoreTake(semBinOff, portMAX_DELAY);
    digitalWrite(LED, LOW);
    vTaskDelay(FRECUENCIA / portTICK_PERIOD_MS );
    xSemaphoreGive(semBinOn);
  }
}
