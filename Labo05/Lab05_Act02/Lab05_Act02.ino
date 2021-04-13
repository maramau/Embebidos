<<<<<<< current
#include <Arduino_FreeRTOS.h>
#include <timers.h>

#define FRECUENCIA4HZ 250     //4Hz
#define FRECUENCIA2HZ 500     //2Hz
#define FRECUENCIA1HZ 1000    //1Hz
#define FRECUENCIA05HZ 2000   //0.5Hz
#define FRECUENCIAREADER 1000 //1Hz
#define IDBLINK1 1
#define IDBLINK2 2

TimerHandle_t handlerBlink2 = (TimerHandle_t) malloc(sizeof(TimerHandle_t));
TimerHandle_t handlerBlink1 = (TimerHandle_t) malloc(sizeof(TimerHandle_t));

//Defino el prototipo de la tarea que leera por consola
void SerialReader(void *pvParameters);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }

  //Tarea que obtendra los caracteres que ingresemos por consola
  xTaskCreate(
    SerialReader
    ,  "SerialReader"
    ,  128
    ,  NULL
    ,  2
    ,  NULL );

  //Seteo los pines de los leds como salida
  DDRB |= (1<<DDB5);
  DDRD |= (1<<DDD5);
  
  //Creo manejadores de timers
  handlerBlink1 = xTimerCreate(
                                  "Blink1",
                                  pdMS_TO_TICKS(FRECUENCIA4HZ),
                                  pdTRUE,  //Asi el timer se reinicia solo
                                  IDBLINK1,
                                  callbackTimer1);
  handlerBlink2 = xTimerCreate(
                                  "Blink2",
                                  pdMS_TO_TICKS(FRECUENCIA1HZ),
                                  pdTRUE,  //Asi el timer se reinicia solo
                                  IDBLINK2,
                                  callbackTimer2);
  //Inicio los manejadores de los timers
  if(handlerBlink1 != NULL){
      if(xTimerStart(handlerBlink1, 0) != pdPASS){
        Serial.println("Fallo el blink1");
      }
  }
  if(handlerBlink2 != NULL){
      if(xTimerStart(handlerBlink2, 0) != pdPASS){
        Serial.println("Fallo el blink2");
      }
  }

  //Inicio los timers. Si no hay RAM suficiente para reservarlos se pasa al siguiente comando
  vTaskStartScheduler();
  Serial.println("RAM insuficiente");
}

void loop(){
}

/*--------------------------------------------------*/
/*-------------------- Callbacks -------------------*/
/*--------------------------------------------------*/

void callbackTimer1(TimerHandle_t xTimer){
  PORTB ^= (1<<PB5);
}

void callbackTimer2(TimerHandle_t xTimer){
  PORTD ^= (1<<PD5);
}

void SerialReader(void *pvParameters){
  (void) pvParameters;
  char opcion = 0;

  while(1){
    vTaskDelay(FRECUENCIAREADER / portTICK_PERIOD_MS );
  
    if(Serial.available()>0){
      opcion = Serial.read();
      switch(opcion){
        case '1':
          xTimerChangePeriod(handlerBlink2,
                             pdMS_TO_TICKS(FRECUENCIA05HZ),
                             50);
          break;
        case '2':
          xTimerChangePeriod(handlerBlink2,
                             pdMS_TO_TICKS(FRECUENCIA2HZ),
                             50);
          break;
        default:
          xTimerChangePeriod(handlerBlink2,
                             pdMS_TO_TICKS(FRECUENCIA1HZ),
                             50);
          break;
        }
      }
   }
}
=======
#include <Arduino_FreeRTOS.h>
#include <timers.h>

#define FRECUENCIA4HZ 250     //4Hz
#define FRECUENCIA2HZ 500     //2Hz
#define FRECUENCIA1HZ 1000    //1Hz
#define FRECUENCIA05HZ 2000   //0.5Hz
#define FRECUENCIAREADER 1000 //1Hz
#define IDBLINK1 1
#define IDBLINK2 2

TimerHandle_t handlerBlink2 = (TimerHandle_t) malloc(sizeof(TimerHandle_t));
TimerHandle_t handlerBlink1 = (TimerHandle_t) malloc(sizeof(TimerHandle_t));

//Defino el prototipo de la tarea que leera por consola
void SerialReader(void *pvParameters);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }

  //Tarea que obtendra los caracteres que ingresemos por consola
  xTaskCreate(
    SerialReader
    ,  "SerialReader"
    ,  128
    ,  NULL
    ,  2
    ,  NULL );

  //Seteo los pines de los leds como salida
  DDRB |= (1<<DDB5);
  DDRD |= (1<<DDD5);

  //Creo manejadores de timers
  handlerBlink1 = xTimerCreate(
                                  "Blink1",
                                  pdMS_TO_TICKS(FRECUENCIA4HZ),
                                  pdTRUE,  //Asi el timer se reinicia solo
                                  IDBLINK1,
                                  callbackTimer1);
  handlerBlink2 = xTimerCreate(
                                  "Blink2",
                                  pdMS_TO_TICKS(FRECUENCIA1HZ),
                                  pdTRUE,  //Asi el timer se reinicia solo
                                  IDBLINK2,
                                  callbackTimer2);
  //Inicio los manejadores de los timers
  if(handlerBlink1 != NULL){
      if(xTimerStart(handlerBlink1, 0) != pdPASS){
        Serial.println("Fallo el blink1");
      }
  }
  if(handlerBlink2 != NULL){
      if(xTimerStart(handlerBlink2, 0) != pdPASS){
        Serial.println("Fallo el blink2");
      }
  }

  //Inicio los timers. Si no hay RAM suficiente para reservarlos se pasa al siguiente comando
  vTaskStartScheduler();
  Serial.println("RAM insuficiente");
}

void loop(){
}

/*--------------------------------------------------*/
/*-------------------- Callbacks -------------------*/
/*--------------------------------------------------*/

void callbackTimer1(TimerHandle_t xTimer){
  PORTB ^= (1<<PB5);
}

void callbackTimer2(TimerHandle_t xTimer){
  PORTD ^= (1<<PD5);
}

void SerialReader(void *pvParameters){
  (void) pvParameters;
  char opcion = 0;

  while(1){
    vTaskDelay(FRECUENCIAREADER / portTICK_PERIOD_MS );

    if(Serial.available()>0){
      opcion = Serial.read();
      switch(opcion){
        case '1':
          xTimerChangePeriod(handlerBlink2,
                             pdMS_TO_TICKS(FRECUENCIA05HZ),
                             50);
          break;
        case '2':
          xTimerChangePeriod(handlerBlink2,
                             pdMS_TO_TICKS(FRECUENCIA2HZ),
                             50);
          break;
        default:
          xTimerChangePeriod(handlerBlink2,
                             pdMS_TO_TICKS(FRECUENCIA1HZ),
                             50);
          break;
        }
      }
   }
}
>>>>>>> before discard
