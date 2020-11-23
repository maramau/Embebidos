#include <Arduino_FreeRTOS.h>

#define FRECUENCIA1 1000
#define FRECUENCIA2 250
#define LED1 13
#define LED2 5

// define two tasks for Blink
void TaskBlink1(void *pvParameters);
void TaskBlink2(void *pvParameters);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskBlink1
    ,  "Blink1"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskBlink2
    ,  "Blink2"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop(){
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlink1(void *pvParameters){
  (void) pvParameters;
  
  while(1){
    digitalWrite(LED1, HIGH);
    vTaskDelay(FRECUENCIA1 / portTICK_PERIOD_MS );
    digitalWrite(LED1, LOW);
    vTaskDelay(FRECUENCIA1 / portTICK_PERIOD_MS );
  }
}

void TaskBlink2(void *pvParameters){
  (void) pvParameters;
  
  while(1){
    digitalWrite(LED2, HIGH);
    vTaskDelay(FRECUENCIA2 / portTICK_PERIOD_MS );
    digitalWrite(LED2, LOW);
    vTaskDelay(FRECUENCIA2 / portTICK_PERIOD_MS );
  }
}
