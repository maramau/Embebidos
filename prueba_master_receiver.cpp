#include "mraa.hpp"
#include "String.h"

#include <iostream>
using namespace std;

#define TAMANO_BUFFER 35		//Como maximo habra
															//4 caracteres separadores (4 bytes)
															//+ 1 char del tamano (1 bytes)
															//+ 14 chars del tipo de mensaje (14 bytes)
															//+ 4 floats (16 bytes)

int main() {
	// Inicializar led conectado a GPIO y controlador de I2C
	mraa::Gpio* d_pin = NULL;

	mraa_platform_t platform = mraa_get_platform_type();
	switch (platform) {
		case MRAA_INTEL_GALILEO_GEN1:
			d_pin = new mraa::Gpio(3, true, true);
			break;
		case MRAA_INTEL_GALILEO_GEN2:
			d_pin = new mraa::Gpio(13, true, false);
			break;
		case MRAA_INTEL_EDISON_FAB_C:
			d_pin = new mraa::Gpio(13, true, false);
			break;
		default:
			std::cerr << "Unsupported platform, exiting" << std::endl;
			return MRAA_ERROR_INVALID_PLATFORM;
	}
	if (d_pin == NULL) {
		std::cerr << "Can't create mraa::Gpio object, exiting" << std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}

	// set the pin as output
	if (d_pin->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
		std::cerr << "Can't set digital pin as output, exiting" << std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}



		//Controlador del I2C que sera usado para transmitir los mensajes
    mraa::I2c* i2c;
    i2c = new mraa::I2c(0);
    i2c->address(8);	//La direccion del esclavo es 8

    uint8_t rx_tx_buf[TAMANO_BUFFER];
    uint8_t tamano_mensaje;	//Tamano del mensaje ()
	uint8_t tipo_mensaje[] = "OBTENER_PROM";
	memcpy((uint8_t *) tipo_mensaje, (const void *)"OBTENER_PROM", sizeof("OBTENER_PROM"));

	uint8_t puntero_mensaje = 0;
	uint8_t i = 0;
	uint8_t tamano_esperado = 4+1+sizeof("RESPONDER_PROM")+4;
	tamano_esperado = 35;

    // Indefinidamente
    for (;;) {
			//Elijo el mensaje a enviar
				//Deberia pedirle al usuario que ingrese alguna opcion por consola?
				//Dependiendo de lo que elija el usuario elegire el valor de tamano_esperado
    			//y el valor de tipo_mensaje y tamano_esperado

    			//COnsiderar hacer una funcion crearMensaje()

    	//Armo el paquete
    	rx_tx_buf[0]='*';
    		//Agregare el tamano del mensaje mas adelante
    	rx_tx_buf[2]='/';
		puntero_mensaje = 3;
		i = 0;
		//Agrego el tipo del mensaje
		while(tipo_mensaje[i]!='\0'){
			rx_tx_buf[puntero_mensaje] = tipo_mensaje[i];
			puntero_mensaje++;
			i++;
		}
		//Como mando mensajes OBTENER no hay payload
		//Agrego el caracter final
		rx_tx_buf[puntero_mensaje] = '*';

		//Agrego el tamano total del mensaje
		tamano_mensaje=puntero_mensaje+1;
		rx_tx_buf[1]= tamano_mensaje;

		//Enviar por I2C
		i2c->write(rx_tx_buf, tamano_mensaje);

		// Apagar led y recibir por I2C
    	sleep(1);
    	d_pin->write(0);
    	i2c->read(rx_tx_buf, tamano_esperado);



		//Aca puedo llamar a una funcion que tome el mensaje leido y lo separe en variables para mostrarlas por pantalla


    	// Luego de un segundo, encender led e imprimir por stdout
    	sleep(1);
    	d_pin->write(1);
    	printf("%i\n", rx_tx_buf[26]);

    	// Forzar la salida de stdout
    	fflush(stdout);
    }

    return 0;
}
