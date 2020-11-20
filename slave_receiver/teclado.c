#include <stdint.h>
#include "teclado.h"
#include <avr/interrupt.h>
#include "fnqueue.h"

#define debounceDelay 50

static conf configTeclado;
uint16_t adc_key_val[5] = {100, 230, 450, 535, 800};
int8_t botonDown = -1;
int16_t buttonState = -1;
int16_t lastButtonState = -1;

struct fun_boton{
  void (*callback_keyUp)();
  void (*callback_keyDown)();
} typedef funBoton;

funBoton arrayBotones[5];

//Guardo la funcion de callback correspondiente a presionar cierto boton
void key_down_callback(void *handler(), int boton){
  if (boton >= 0 && boton < CANT_BOTONES){
    arrayBotones[boton].callback_keyDown = handler;
  }
}

//Guardo la funcion de callback correspondiente a soltar cierto boton
void key_up_callback(void *handler(), int boton){
  if (boton >= 0 && boton < CANT_BOTONES){
    arrayBotones[boton].callback_keyUp = handler;
  }
}

//Espero un tiempo entre una lectura y otra para evitar leer ruido
  //ocasionado por la cercania de un par de metales
void debounce(){
		buttonState = configTeclado->ultMedicion;

		if (buttonState != lastButtonState){
			delay(debounceDelay);
			buttonState = PIND & (1<<PD2);
			if (buttonState != lastButtonState){
        //Se aprieta la tecla
				if (!buttonState){//Se solto la tecla apretada
					funcionABoton();
				}

				//Si hubo cambios válidos actualizo el valor de lectura anterior
				lastButtonState = buttonState;
			}
		}
}

//Al terminar una conversion se llama a esta funcion para ejecutar la funcion correspondiente
void funcionABoton(){
  int8_t i = 0;
  bool encontre = false;
  
  while (!encontre && i < CANT_BOTONES){
    if (configTeclado->ultMedicion < adc_key_val[i]){
      arrayBotones[i].callback_keyDown();
      encontre = true;
      botonDown = i;
    }
    i++;
  }

  if (botonDown >= 0){
      arrayBotones[botonDown].callback_keyUp();
      botonDown = -1;
  }
}


//Seteo inicialmente la configuracion del teclado
conf teclado_setup(){
  int outputTeclado = 0;
  
  configTeclado = (conf)malloc(sizeof(struct adc_cfg)); //El LCD no se prende si no lo hago
                                                        //Se estara reescribiendo la memoria dinamica de la estructura?
  configTeclado->canal = 2;
  configTeclado->ultMedicion = 0;
  configTeclado->confActual = 2;
  configTeclado->callback = debounce;
  outputTeclado = adc_init(configTeclado);
  
  return configTeclado;
}
