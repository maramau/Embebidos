#include "adc.h"
#include "fnqueue.h"
#include "critical.h"
#include "Arduino.h"

static volatile int16_t valorLeido;
//Arreglo que contiene dos configuraciones (una por canal)
conf lasConfig[CANT_CANALES];

//Asigno un lugar en el arreglo de configuraciones a la configuracion dada por parametro
  //segun el numero del canal elegido
int adc_init(conf cfg){
  int salida = 0;
  
  if (lasConfig[cfg->canal] == NULL){
    lasConfig[cfg->canal] = cfg;
    salida = 1;
  }
  
  return salida;
}

int adc_setup(){
  //inicializarConfigs();
  
  ADMUX &= ~(1 << ADLAR);
  ADMUX |= (1 << REFS0);
  ADMUX &= ~(1 << MUX3 | 1 << MUX2 | 1 << MUX1 | 1 << MUX0);
  //ADMUX |= 1 << MUX1;
  
  ADCSRB &= ~(1 << ADTS2 | 1 << ADTS1 | 1 << ADTS0);

  ADCSRA |= (1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0);
  ADCSRA |= 1 << ADIE;
  ADCSRA |= 1 << ADEN;
  ADCSRA |= 1 << ADSC;  //Inicio una conversion en modo single run
  sei();

  return 0;
}

int obtenerConfiguracionActiva(){
  uint8_t i = 0;
  uint8_t encontre = 0;
  while (i < CANT_CANALES && !encontre){
    if (lasConfig[i]->confActual){
      encontre = 1;
    }
    else{
      i++;
    }
  }

  return i;
}

void adc_loop(){
  //Obtengo las posiciones en el arreglo de las configuraciones
    //del canal activo y el que le sigue ejecutar
  uint8_t idConfActual = obtenerConfiguracionActiva();
  uint8_t idConfSig = (idConfActual + 1) % CANT_CANALES;

  //Obtengo las configuraciones
  conf actual = lasConfig[idConfActual];
  conf siguiente = lasConfig[idConfSig];
  
  //Almaceno el útlimo valor leído de la ocnfiguracion actual
  critical_begin();
  actual->ultMedicion = valorLeido;
  critical_end();
  
  //Ejecuto el callback de la configuracion actual
  actual->callback();

  //Cambio el canal
  cambiarCanal(actual, siguiente);
  delay(1);
  //Inicio una nueva conversión
  ADCSRA |= 1 << ADSC;
}

void inicializarConfigs(){
  int i = 0;
  for (i = 0; i < CANT_CANALES; i++){
    lasConfig[i] = NULL;
  }
}

void cambiarCanal(conf canalActivo, conf canalSig){
  uint8_t next = canalSig->canal;
  
  if (canalActivo != NULL && canalSig != NULL){
    //Seteo el ADMUX segun el siguiente canal a usar
    ADMUX &= ~(1 << MUX3 | 1 << MUX2 | 1 << MUX1 | 1 << MUX0);
    switch (next){
      case 0:
        //ADMUX |= 1 << MUX1;
        break;
      case 1:
        ADMUX |= 1 << MUX0;
        break;
      case 2:
        ADMUX |= 1 << MUX1;
        break;
    }
    canalActivo->confActual = 0;
    canalSig->confActual = 1;
  }
}

ISR(ADC_vect){
  valorLeido = (ADCL) | (ADCH << 8);
  fnqueue_add(adc_loop);
}
