#include <stdint.h>
#include "teclado.h"
#include <avr/interrupt.h>
#include "fnqueue.h"

#define debounceDelay  50

static myCfg configTeclado;
uint16_t adc_key_val[5] = {50, 230, 450, 535, 800};
int8_t teclaApretada = -1;
int16_t buttonState = -1;
int16_t lastButtonState = -1;
bool foundKey;

/*
struct structMyKey
{
  void (*callback_keyUp)();
  void (*callback_keyDown)();
} typedef myKey;*/

myKey lasTeclas[5];

void key_down_callback(void *handler(), int tecla)
{
  if (tecla >= 0 && tecla < CANT_TECLAS)
  {
    lasTeclas[tecla].callback_keyDown = handler;
  }
}

void key_up_callback(void *handler(), int tecla)
{
  if (tecla >= 0 && tecla < CANT_TECLAS)
  {
    lasTeclas[tecla].callback_keyUp = handler;
  }
}

void debounce(){
		//Leer el estado del pulsador
		buttonState = configTeclado->valorObtenido;

		//Si cambió el estado del pulsador...
		if (buttonState != lastButtonState)
		{
			//Se espera un tiempo para evitar leer el ruido del rebote del pulsador
			delay(debounceDelay);

			//Se vuelve a leer el estado del pulsador
			buttonState = PIND & (1<<PD2);

			//Si el cambio se mantiene, se interpreta como evento de keydown/keyup válido
			if (buttonState != lastButtonState)
			{
				//El pulso es válido, se implementa la lógica del sistema

				//Si no está presionado el pulsador (se detectó un keyup)...
				if (!buttonState)
				{					
					leerTecla(); //Se modifica el estado del led (toggle vía XOR)
				}

				//Si hubo cambios válidos actualizo el valor de lectura anterior
				lastButtonState = buttonState;
			}
		}
}

//Esta función se ejecuta siempre que termina una conversion AD!
void leerTecla()
{
  //Dado que se pulsó una tecla, hay que determinar cual fue
  int8_t i = 0;
  bool foundKey = false;
  while (!foundKey && i < CANT_TECLAS)
  {
    if (configTeclado->valorObtenido < adc_key_val[i])
    {

      lasTeclas[i].callback_keyDown();
      foundKey = true;
      teclaApretada = i;
    }
    else
      i++;
  }

  if (teclaApretada >= 0)
  {
    if (configTeclado->valorObtenido > 900)
    {
      lasTeclas[teclaApretada].callback_keyUp();
      teclaApretada = -1;
    }
  }
}

void funcionFalsa()
{
}

myCfg teclado_setup()
{
  configTeclado = (myCfg)malloc(sizeof(struct adc_cfg));
  configTeclado->canal = 0;
  configTeclado->valorObtenido = 0;
  configTeclado->configActiva = 0;
  configTeclado->callback = debounce;

  int outputTeclado = adc_init(configTeclado);

  int j;

  for (j = 0; j < 5; j++)
  {
    lasTeclas[j].callback_keyDown = funcionFalsa;
    lasTeclas[j].callback_keyUp = funcionFalsa;
  }
  return configTeclado;
}
