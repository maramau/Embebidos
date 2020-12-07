#include "adc.h"
#include "fnqueue.h"
#include "critical.h"
#include "Arduino.h"

static volatile int16_t valorLeido;
//Arreglo que contiene dos configuraciones (una por canal)
conf lasConfig[CANALES];

int adc_init(conf cfg)
{
  int salida = 0;
  if (lasConfig[cfg->canal] == NULL)
  {
    lasConfig[cfg->canal] = cfg;
    salida = 1;
  }
  return salida;
}

int adc_setup()
{
  ADMUX &= ~(1 << ADLAR);
  ADMUX |= (1 << REFS0);

  //Leer sensor
  ADMUX &= ~(1 << MUX3 | 1 << MUX2 | 1 << MUX1 | 1 << MUX0);
  ADMUX |= 1 << MUX0;

  ADCSRA |= 1 << ADEN;

  ADCSRB &= ~(1 << ADTS2 | 1 << ADTS1 | 1 << ADTS0);

  ADCSRA |= (1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0);

  ADCSRA |= 1 << ADIE;

  sei();

  inicializarConfigs();
  ADCSRA |= 1 << ADSC;
  return 0;
}

conf obtenerConfiguracionActiva()
{
  uint8_t i = 0;
  uint8_t encontre = 0;
  conf salida = NULL;
  while (i < CANALES && !encontre)
  {
    if (lasConfig[i]->confActual)
    {
      encontre = 1;
      salida = lasConfig[i];
    }
    else
    {
      i++;
    }
  }

  return salida;
}

void adc_loop()
{
  //Obtengo el canal activo
  conf actual = obtenerConfiguracionActiva();
  //Almaceno el útlimo valor leído
  critical_begin();
  actual->ultMedicion = valorLeido;
  critical_end();
  //Ejecuto la función correspondiente
  actual->callback();

  cambiarCanal(actual);
  delay(1);
  //Inicio una nueva conversión
  ADCSRA |= 1 << ADSC;
}

void inicializarConfigs()
{
  int i = 0;
  for (i = 0; i < CANALES; i++)
  {
    lasConfig[i] = NULL;
  }
}

void cambiarCanal(conf canalActivo)
{
  if (canalActivo != NULL)
  {
    if (canalActivo->canal == 0)
    {
      ADMUX &= ~(1 << MUX3 | 1 << MUX2 | 1 << MUX1 | 1 << MUX0);
      ADMUX |= 1 << MUX0;
      canalActivo->confActual = 0;
      lasConfig[1]->confActual = 1;
    }
    else
    {
      if (canalActivo->canal == 1)
      {
        ADMUX &= ~(1 << MUX3 | 1 << MUX2 | 1 << MUX1 | 1 << MUX0);
        canalActivo->confActual = 0;
        lasConfig[0]->confActual = 1;
      }
    }
  }
}

ISR(ADC_vect)
{

  valorLeido = (ADCL) | (ADCH << 8);
  fnqueue_add(adc_loop);
 
}
