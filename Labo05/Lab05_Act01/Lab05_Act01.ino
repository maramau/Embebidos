#include <Arduino_FreeRTOS.h>

#define FRECUENCIA 1000
#define LED 13

// define two tasks for Blink
void TaskBlinkOn(void *pvParameters);
void TaskBlinkOff(void *pvParameters);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }
  pinMode(LED, OUTPUT);

  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskBlinkOn
    ,  "BlinkOn"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskBlinkOff
    ,  "BlinkOff"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

  // Now the task scheduler, which takes over control of scheduling individual tasks
  vTaskStartScheduler();
}

void loop(){
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlinkOn(void *pvParameters){
  (void) pvParameters;

  while(1){
    digitalWrite(LED, HIGH);
    vTaskDelay(FRECUENCIA / portTICK_PERIOD_MS );
  }
}

void TaskBlinkOff(void *pvParameters){
  (void) pvParameters;
  vTaskDelay((FRECUENCIA/2) / portTICK_PERIOD_MS ); //Retraso inicial para que las tareas no se ejecuten a la vez

  while(1){
    digitalWrite(LED, LOW);
    vTaskDelay(FRECUENCIA / portTICK_PERIOD_MS );
  }
}
