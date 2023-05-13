
#include "nuevo_perfil.h"

/*-----------------------NUEVO PERFIL-------------------------*/
typedef enum {LETRA1, LETRA2, LETRA3} t_perfil;
t_perfil n_letra;

extern int menu;
extern int perfil;
extern void agregarPerfil(char nombre[]);

char abecedario[26] = {"a""b""c""d""e""f""g""h""i""j""k""l""m""n""o""p""q""r""s""t""u""v""w""x""y""z"};

char nombre[3];
int selector_letra = 0;

int confirmacion = 0;
int guardar = 1;
char final [10];


void nuevo_perfil(void) {
	encender_led(PIN_BLUE);
	sprintf(nombre, "aaa");
	EscribirFrase_L1("Introduce 3 letras:");
	EscribirFrase_L2(nombre);

	while(perfil) {
		osEvent evtent = osSignalWait(0, osWaitForever);
		if(evtent.status == osEventSignal) {
			switch(evtent.value.signals) {
				case PULSACION_DOWN: //Avanzar en el abecedario
					avanzarABC();
					break;

				case PULSACION_UP: // Retroceder en el abecedario
					retrocederABC();
					break;

				case PULSACION_LEFT: //Retroceder al menu principal
					retrocederPrincipal();
					break;

				case PULSACION_RIGHT: //Confirmar letra y pasar a siguiente
					siguienteLetra();
					break;

				case PULSACION_CENTER: //Retroceder a modificar la letra anterior
					modificarAnterior();
					break;

			}//Cierre switch pulsador

			if(confirmacion) {
				sprintf(final, "Hola %s,", nombre);
				EscribirFrase_L1(final);
				EscribirFrase_L2("perfil creado OK");

				if(guardar) {
					guardar = 0;
					agregarPerfil(nombre);
				}
				osDelay(1500);
				retrocederPrincipal();

			}

		}//Cierre if

	} //Cierre mini_juegos = 1

}

void retrocederPrincipal() {
	//Reset de todas las variables
	menu = 1;
	perfil = 0;
	n_letra = LETRA1;
	selector_letra = 0;
	confirmacion = 0;
	sprintf(nombre, "aaa");

}

void siguienteLetra() {
	if(confirmacion == 0) {
		switch(n_letra) {
			case LETRA1:
				n_letra = LETRA2;
				selector_letra = 0;
				nombre [1] = abecedario[0];
				EscribirFrase_L2(nombre);
				break;

			case LETRA2:
				n_letra = LETRA3;
				selector_letra = 0;
				nombre [2] = abecedario[0];
				EscribirFrase_L2(nombre);
				break;

			case LETRA3:
				n_letra = LETRA1;
				selector_letra = 0;
				confirmacion = 1;
				guardar = 1;
				break;
		}
	}
}

void retrocederABC() {
	if(confirmacion == 0) {
		if(selector_letra != 0) {
			selector_letra --;
		}
		switch(n_letra) {
			case LETRA1:
				nombre [0] = abecedario[selector_letra];
				break;

			case LETRA2:
				nombre [1] = abecedario[selector_letra];
				break;

			case LETRA3:
				nombre [2] = abecedario[selector_letra];
				break;

		}
		EscribirFrase_L2(nombre);

	}

}

void avanzarABC() {
	if(confirmacion == 0) {
		if(selector_letra != 25) {
			selector_letra ++;
		}
		switch(n_letra) {
			case LETRA1:
				nombre [0] = abecedario[selector_letra];
				break;

			case LETRA2:
				nombre [1] = abecedario[selector_letra];
				break;

			case LETRA3:
				nombre [2] = abecedario[selector_letra];
				break;
		}
		EscribirFrase_L2(nombre);
	}
}

void modificarAnterior() {
	switch(n_letra) {
		case LETRA1:
			selector_letra = 0;
			nombre [0] = abecedario[0];
			EscribirFrase_L2(nombre);
			break;

		case LETRA2:
			n_letra = LETRA1;
			selector_letra = 0;
			nombre [0] = abecedario[0];
			nombre [1] = abecedario[0];
			EscribirFrase_L2(nombre);
			break;

		case LETRA3:
			n_letra = LETRA2;
			selector_letra = 0;
			nombre [1] = abecedario[0];
			nombre [2] = abecedario[0];
			EscribirFrase_L2(nombre);
			break;
	}
}
