#include "device.h"
#include "fnqueue.h"
#include "critical.h"
#include "Arduino.h"

#define CANT_CANALES 8

static volatile int16_t analogValSensor;

conf arrayConf[2];

//Obtengo una configuracion para el sensor pasada por parametro 
  //y la guardo en mi arreglo de configuraciones para ambos canales
int adc_init(conf cfg){
  int salida = 0;
  
  if ((arrayConf[cfg->canal] == NULL) || (arrayConf[cfg->canal] > CANT_CANALES) || (arrayConf[cfg->canal] < 0)){
    arrayConf[cfg->canal] = cfg;
    salida = 1;
  }
  return salida;
}

//Configuracion inicial del ADC
int adc_setup(){
// clear ADLAR in ADMUX (0x7C) to right-adjust the result
  // ADCL will contain lower 8 bits, ADCH upper 2 (in last two bits)
  ADMUX &= ~(1<<ADLAR);
  
  // Set REFS1..0 in ADMUX (0x7C) to change reference voltage to the
  // proper source (01)
  ADMUX |= (1<<REFS0);
  
  // Clear MUX3..0 in ADMUX (0x7C) in preparation for setting the analog input
  ADMUX &= ~(1<<MUX3 | 1<<MUX2 | 1<<MUX1 | 1<<MUX0);
  ADMUX |= (1<<MUX0);
  // Set MUX3..0 in ADMUX (0x7C) to read from AD8 (Internal temp)
  // Do not set above 15! You will overrun other parts of ADMUX. A full
  // list of possible inputs is available in Table 24-4 of the ATMega328
  // datasheet
  
  // Set ADEN in ADCSRA (0x7A) to enable the ADC.
  // Note, this instruction takes 12 ADC clocks to execute
      ADCSRA |= 1<<ADEN;
  
  //Set ADATE in ADCSRA (0x7A) to disable auto-triggering.
  //El registro ADATE es el que inicia la conversión por un evento externo, o de forma automática en modo free running. 
  //Si vas a leer varios 2, ADATE tiene que estar en 0 (single conversion)
  ADCSRA &= ~(1<<ADATE);
  // Clear ADTS2..0 in ADCSRB (0x7B) to set trigger mode to free running.
  // This means that as soon as an ADC has finished, the next will be
  // immediately started.
  ADCSRB &= ~(1<<ADTS2 | 1<<ADTS1 | 1<<ADTS0);
  
  // Set the Prescaler to 128 (16000KHz/128 = 125KHz)
  // Above 200KHz 10-bit results are not reliable.
  ADCSRA |= (1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0);
  
  // Set ADIE in ADCSRA (0x7A) to enable the ADC interrupt.
  // Without this, the internal interrupt will not trigger.
  ADCSRA |= 1<<ADIE;
  
  // Kick off the first ADC
  
  // Set ADSC in ADCSRA (0x7A) to start the ADC conversion
  ADCSRA |= 1<<ADSC;
  
  sei();

  ADCSRA |= 1 << ADSC;
  return 0;
}

//Devuelve la estructura de la configuracion actual del canal
conf confActual(){
  conf salida = NULL;
  
  if (arrayConf[0]->confActual){
    salida = arrayConf[0];
  }
  else{
    salida = arrayConf[1];
  }
  
  return salida;
}

//Segun el canal que este usando, llamare una funcion de callback para procesar el ultimo valor leido
void adc_loop(){
  critical_begin();
  
  conf actual = confActual();
  actual->ultMedicion = analogValSensor;
  actual->callback();
  cambiarCanal(actual);
  delay(1);            //Debouncing de 1 ms
                       //Si no lo hago, se lee un valor demasiado alto de temperatura 
                       //a la hora de cambiar el canal
  ADCSRA |= 1 << ADSC; //Inicio una nueva conversión (estoy en single-conversion)
  
  critical_end();
}

//Luego de realizar una lectura del ADC cambio de canal a estilo RR
void cambiarCanal(conf canalActivo){
   ADMUX &= ~(1 << MUX3 | 1 << MUX2 | 1 << MUX1 | 1 << MUX0); //Limpio el registro ADMUX
   if (canalActivo->canal == 0){
     ADMUX |= 1 << MUX0;
     canalActivo->confActual = 0;
     arrayConf[1]->confActual = 1;
   }
   else{
       canalActivo->confActual = 0;
       arrayConf[0]->confActual = 1;
   }
}


//Interrupcion que se ejecuta cuando el ADC termina una conversion
//Cuando se inicia setea los registros ADIF (1) y ADSRA (0)
ISR(ADC_vect){
  analogValSensor = (ADCL) | (ADCH << 8);
  fnqueue_add(adc_loop);
}
