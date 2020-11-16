#include "mraa.hpp"
#include "String.h"

#include <iostream>
using namespace std;

#define TAMANO_MAXIMO_BUFFER 50	//Como maximo habra
									//4 caracteres separadores (4 bytes)
									//+ 1 char del tamano (1 bytes)
									//+ 14 chars del tipo de mensaje (14 bytes)
									//+ 4 floats (16 bytes)

uint8_t tamano_respuesta;
uint8_t tamano_pedido;

//Toma un entero por consola y elije el tipo de mensaje a enviar
	//calcula tambien el tamano esperado de la respuesta en base a la eleccion
char* recibir_pedido(){
	int str;
	char* toReturn;

	printf("Ingrese una opcion:\n 1)Obtener T actual\n 2)Obtener T maxima\n 3)Obtener T minima\n 4)Obtener T promedio\n 5)Obtener todo\n");
	scanf("%i", &str);

	switch(str){
	case 1:
		toReturn = "OBTENER_TEMP";
		tamano_respuesta = 24;
		tamano_pedido =  16;
		break;
	case 2:
		toReturn = "OBTENER_MAX";
		tamano_respuesta = 23;
		tamano_pedido =  15;
		break;
	case 3:
		toReturn = "OBTENER_MIN";
		tamano_respuesta = 23;
		tamano_pedido =  15;
		break;
	case 4:
		toReturn = "OBTENER_PROM";
		tamano_respuesta = 24;
		tamano_pedido =  16;
		break;
	case 5:
		toReturn = "OBTENER_TODO";
		tamano_respuesta = 39;
		tamano_pedido =  16;
		break;
	default:
		toReturn = "Nice try. Zero charisma.\n";
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
	char* tipo_mensaje;

    // Indefinidamente
    for (;;) {
			//Elijo el mensaje a enviar
				//Deberia pedirle al usuario que ingrese alguna opcion por consola?
				//Dependiendo de lo que elija el usuario elegire el valor de tamano_respuesta
    			//y el valor de tipo_mensaje y tamano_respuesta

    			//Considerar hacer una funcion crearMensaje()
    	tipo_mensaje = recibir_pedido();
    	if(tamano_respuesta != -1){
			//Armo el paquete
			rx_tx_buf[0]='*';
			puntero_mensaje = 1;
			i = 0;
			//Agrego el tipo del mensaje
			while(tipo_mensaje[i]!='\0'){
				rx_tx_buf[puntero_mensaje] = tipo_mensaje[i];
				puntero_mensaje++;
				i++;
			}
			rx_tx_buf[puntero_mensaje++] = '/';
			//Como mando mensajes OBTENER no hay payload
			//Agrego el tamano total del mensaje
			rx_tx_buf[puntero_mensaje++]= tamano_pedido;
			//Agrego el caracter final
			rx_tx_buf[puntero_mensaje] = '*';

			//Enviar por I2C
			i2c->write(rx_tx_buf, tamano_pedido);

			// Apagar led y recibir por I2C
			sleep(1);
			d_pin->write(0);
			i2c->read(rx_tx_buf, tamano_respuesta);



			//Aca puedo llamar a una funcion que tome el mensaje leido y lo separe en variables para mostrarlas por pantalla
			i=0;
			uint8_t temp[TAMANO_MAXIMO_BUFFER];
			while(i<tamano_respuesta){
				temp[i]=rx_tx_buf[i];
				i++;
			}
			temp[i]='\0';
			// Luego de un segundo, encender led e imprimir por stdout
			sleep(1);
			d_pin->write(1);
			//float f1, f2;
			//f1 = (float) rx_tx_buf[16];
			//f2 = (float) rx_tx_buf[17];
			printf("Respuesta: %c %c %c\n",rx_tx_buf[21], rx_tx_buf[22],rx_tx_buf[23]);
			printf("%s\n", temp);

			// Forzar la salida de stdout
			fflush(stdout);
    	}
    }

    return 0;
}
