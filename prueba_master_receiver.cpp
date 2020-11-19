#include <common.h>
#include <gpio.hpp>
#include <i2c.hpp>
#include <types.h>
#include <unistd.h>
#include <cstdint>
#include <cstdio>
#include <iostream>

using namespace std;

#define TAMANO_MAXIMO_BUFFER 80	//Como maximo habra
									//4 caracteres separadores (4 bytes)
									//+ 1 char del tamano (1 bytes)
									//+ 14 chars del tipo de mensaje (14 bytes)
									//+ 4 floats (16 bytes)
#define OBTENER_TEMP 1
#define OBTENER_MIN 2
#define OBTENER_MAX 3
#define OBTENER_PROM 4
#define OBTENER_TODO 5
#define RESPONDER_TEMP 6
#define RESPONDER_MIN 7
#define RESPONDER_MAX 8
#define RESPONDER_PROM 9
#define RESPONDER_TODO 10

uint8_t tamano_respuesta;
uint8_t tamano_pedido;

void limpiar_buffer(uint8_t* buffer){
	uint8_t i = 0;
	while (buffer[i]!='\0'){
		buffer[i++] = '\0';
	}
}

//Toma un entero por consola y elije el tipo de mensaje a enviar
	//calcula tambien el tamano esperado de la respuesta en base a la eleccion
uint8_t recibir_pedido(){
	int str;
	uint8_t toReturn;

	printf("Ingrese una opcion:\n 1)Obtener T actual\n 2)Obtener T maxima\n 3)Obtener T minima\n 4)Obtener T promedio\n 5)Obtener todo\n");
	scanf("%i", &str);

	switch(str){
	case 1:
		toReturn = OBTENER_TEMP;
		tamano_respuesta = 24;
		tamano_pedido =  16;
		break;
	case 2:
		toReturn = OBTENER_MIN;
		tamano_respuesta = 23;
		tamano_pedido =  15;
		break;
	case 3:
		toReturn = OBTENER_MAX;
		tamano_respuesta = 23;
		tamano_pedido =  15;
		break;
	case 4:
		toReturn = OBTENER_PROM;
		tamano_respuesta = 24;
		tamano_pedido =  16;
		break;
	case 5:
		toReturn = OBTENER_TODO;
		tamano_respuesta = 39;
		tamano_pedido =  16;
		break;
	default:
		toReturn = -1;
		tamano_respuesta = -1;
	}

	return toReturn;
}



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

    uint8_t rx_tx_buf[TAMANO_MAXIMO_BUFFER];
	uint8_t puntero_mensaje = 0;
	uint8_t i = 0;
	uint8_t tipo_mensaje;

    // Indefinidamente
    for (;;) {
			//Elijo el mensaje a enviar
				//Deberia pedirle al usuario que ingrese alguna opcion por consola?
				//Dependiendo de lo que elija el usuario elegire el valor de tamano_respuesta
    			//y el valor de tipo_mensaje y tamano_respuesta

    			//Considerar hacer una funcion crearMensaje()
    	//limpiar_buffer(rx_tx_buf);
    	tipo_mensaje = recibir_pedido();
    	if(tamano_respuesta != -1){
			//Armo el paquete
			rx_tx_buf[0]='*';
			puntero_mensaje = 1;
			i = 0;
			//Agrego el tipo del mensaje
			rx_tx_buf[puntero_mensaje++] = tipo_mensaje;
			rx_tx_buf[puntero_mensaje++] = '/';
			//Como mando mensajes OBTENER no hay payload
			//Agrego el tamano total del mensaje
			rx_tx_buf[puntero_mensaje++] = tamano_pedido;
			//Agrego el caracter final
			rx_tx_buf[puntero_mensaje] = '*';

			//Enviar por I2C
			printf("Escribo: %s\n",rx_tx_buf);
			i2c->write(rx_tx_buf, tamano_pedido);

			// Apagar led y recibir por I2C
			sleep(1);
			d_pin->write(0);

			int leidos = i2c->read(rx_tx_buf, tamano_respuesta);
			printf("Bytes leidos:%i\n", leidos);

			// Luego de un segundo, encender led e imprimir por stdout
			rx_tx_buf[leidos-1] = '\0';
			sleep(1);
			d_pin->write(1);
			printf("Mensaje: %s\n Tamaño: %i\n Separador: %c\n", rx_tx_buf, rx_tx_buf[tamano_respuesta-1], rx_tx_buf[tamano_respuesta]);

			// Forzar la salida de stdout
			fflush(stdout);
    	}
    }

    return 0;
}
