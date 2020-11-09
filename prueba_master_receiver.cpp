#include "mraa.hpp"
#include "String.h"

#include <iostream>
using namespace std;

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

    uint8_t rx_tx_buf[11];	//Por lo que calcule el tamaño maximo sera de 81 bytes
														//Este numero podria cambiar

    // Indefinidamente
    for (;;) {

			//Enviar por I2C
    	strcpy((char*) rx_tx_buf,(const char *) String("*")+size;
			i2c->write(rx_tx_buf, sizeof(rx_tx_buf));

    	// Apagar led y recibir por I2C
    	sleep(1);
    	d_pin->write(0);
    	i2c->read(rx_tx_buf, 6);

    	// Luego de un segundo, encender led e imprimir por stdout
    	sleep(1);
    	d_pin->write(1);
    	printf((char *) rx_tx_buf, "%s\n");

    	// Forzar la salida de stdout
    	fflush(stdout);
    }

    return 0;
}
