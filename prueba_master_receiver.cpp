#include "mraa.hpp"
#include "String.h"
#include <iostream>

using namespace std;

#define TAMANO_MAXIMO_FLOAT 5	//Cada float sera representado por 5 chars
									//2 digitos del lado izquierdo del punto
									//1 punto
									//2 digitos del lado derecho del punto

#define TAMANO_UNICO_PEDIDO 5		//Como maximo habra
										//3 separadores (3 bytes)
										//+1 char del tipo de mensaje (1 byte)
										//+1 char del tamano del mensaje (1 byte)
#define TAMANO_MAXIMO_RESPUESTA 30	//Como maximo habra
										//4 caracteres separadores (4 bytes)
										//+1 char del tamano (1 bytes)
										//+1 char del tipo de mensaje (1 bytes)
										//+4 floats con 3 separadores (4*5+3=23 bytes)
										//+1 caracter nulo final?
#define OBTENER_TEMP '1'
#define OBTENER_MIN '2'
#define OBTENER_MAX '3'
#define OBTENER_PROM '4'
#define OBTENER_TODO '5'
#define RESPONDER_TEMP '6'
#define RESPONDER_MIN '7'
#define RESPONDER_MAX '8'
#define RESPONDER_PROM '9'
#define RESPONDER_TODO '0'

#define TAMANO_FLOAT 8

#define CARACTER_INI_FIN '*'
#define CARACTER_SEPARADOR '/'

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
		break;
	case 2:
		toReturn = OBTENER_MIN;
		break;
	case 3:
		toReturn = OBTENER_MAX;
		break;
	case 4:
		toReturn = OBTENER_PROM;
		break;
	case 5:
		toReturn = OBTENER_TODO;
		break;
	default:
		toReturn = -1;
	}

	return toReturn;
}


//Aca separo el buffer en variables y las muestro por consola?
//El formato es *tipo/float[-float]/tamano*
	//donde [-float] puede ser vacio o pueden ser 3 floats concatenados con un -
void procesarRespuesta(uint8_t* buffer){
	uint8_t tamano_respuesta, tipo_respuesta;
	uint8_t puntero_mensaje = 1, i = 0;
	char t_act[TAMANO_FLOAT] = {'\0', '\0', '\0', '\0', '\0'};
	char t_min[TAMANO_FLOAT] = {'\0', '\0', '\0', '\0', '\0'};
	char t_max[TAMANO_FLOAT] = {'\0', '\0', '\0', '\0', '\0'};
	char t_prom[TAMANO_FLOAT] = {'\0', '\0', '\0', '\0', '\0'};

	tipo_respuesta = buffer[puntero_mensaje++];
	puntero_mensaje++;		//Salteo el caracter separador

	  switch (tipo_respuesta){

	    case RESPONDER_TEMP:
	      while (buffer[puntero_mensaje] != '/'){
	    	  t_act[i++] = buffer[puntero_mensaje++];
	      }
	      t_act[i]='\0';
    	  printf("Temperatura actual: %s\n",t_act);
	      break;

	    case RESPONDER_MIN:
	      while (buffer[puntero_mensaje] != '/'){
	    	  t_min[i++] = buffer[puntero_mensaje++];
	      }
	      t_min[i]='\0';
	      printf("Temperatura minima: %s\n",t_min);
	      break;

	    case RESPONDER_MAX:
		  while (buffer[puntero_mensaje] != '/'){
			  t_max[i++] = buffer[puntero_mensaje++];
		  }
		  t_max[i]='\0';
		  printf("Temperatura maxima: %s\n",t_max);
	      break;

	    case RESPONDER_PROM:
	      while (buffer[puntero_mensaje] != '/'){
	    	  t_prom[i++] = buffer[puntero_mensaje++];
	      }
	      t_prom[i]='\0';
	      printf("Temperatura promedio: %s\n",t_prom);
	      break;

	    case RESPONDER_TODO:
	      while (buffer[puntero_mensaje] != '-'){
			  t_act[i++] = buffer[puntero_mensaje++];
		  }
	      puntero_mensaje++;		//Salteo el separador
	      t_act[i]='\0';
	      i=0;

		  while (buffer[puntero_mensaje] != '-'){
			  t_min[i++] = buffer[puntero_mensaje++];
		  }
		  puntero_mensaje++;		//Salteo el separador
	      t_min[i]='\0';
	      i=0;

		  while (buffer[puntero_mensaje] != '-'){
			  t_max[i++] = buffer[puntero_mensaje++];
		  }
		  puntero_mensaje++;		//Salteo el separador
	      t_max[i]='\0';
	      i=0;

		  while (buffer[puntero_mensaje] != '/'){
			  t_prom[i++] = buffer[puntero_mensaje++];
		  }
	      t_prom[i]='\0';

	      printf("Temperatura actual: %s\n",t_act);
	      printf("Temperatura minima: %s\n",t_min);
	      printf("Temperatura maxima: %s\n",t_max);
	      printf("Temperatura promedio: %s\n",t_prom);
	      break;
	  }

	  puntero_mensaje++;		//Salteo el caracter separador
	  tamano_respuesta = buffer[puntero_mensaje];
	  printf("Tamano del mensaje: %i\n", tamano_respuesta);
}



int main() {
    uint8_t rx_tx_buf[TAMANO_MAXIMO_RESPUESTA];
	uint8_t puntero_mensaje = 0;
	uint8_t tipo_mensaje;


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

    // Indefinidamente
    for (;;) {
    			//Considerar hacer una funcion crearMensaje()
    	tipo_mensaje = recibir_pedido();
    	if(tipo_mensaje != -1){
    		puntero_mensaje = 0;
			//Armo el paquete
			rx_tx_buf[puntero_mensaje++] = CARACTER_INI_FIN;
			//Agrego el tipo del mensaje
			rx_tx_buf[puntero_mensaje++] = tipo_mensaje;
			rx_tx_buf[puntero_mensaje++] = CARACTER_SEPARADOR;
			//Como mando mensajes OBTENER no hay payload
			//Agrego el tamano total del mensaje
			rx_tx_buf[puntero_mensaje++] = TAMANO_UNICO_PEDIDO;
			//Agrego el caracter final
			rx_tx_buf[puntero_mensaje++] = CARACTER_INI_FIN;
			rx_tx_buf[puntero_mensaje] = '\0';
			//Enviar por I2C
			printf("Escribo: %s\n",rx_tx_buf);
			i2c->write(rx_tx_buf, TAMANO_UNICO_PEDIDO);
			// Apagar led y recibir por I2C
			sleep(1);
			d_pin->write(0);

			i2c->read(rx_tx_buf, TAMANO_MAXIMO_RESPUESTA);
			// Luego de un segundo, encender led e imprimir por stdout
			sleep(1);
			d_pin->write(1);
			//printf("Mensaje: %s\n Tamaño: %i\n Separador: %c\n", rx_tx_buf, rx_tx_buf[TAMANO_MAXIMO_RESPUESTA], rx_tx_buf[TAMANO_MAXIMO_RESPUESTA]);

			//Descifro el mensaje y muestro las temperaturas obtenidas por pantalla
			procesarRespuesta(rx_tx_buf);

			// Forzar la salida de stdout
			fflush(stdout);
    	}
    }

    return 0;
}
