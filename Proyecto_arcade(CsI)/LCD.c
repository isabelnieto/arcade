#include "LCD.h"
#include "Arial12x12.h"
#include "numbers.h"

uint8_t buffer [128];

uint16_t posicionL1;
uint16_t posicionL2;


void retardo_1us(void) {
	uint32_t cont1u;
	for(cont1u = 0; cont1u < 25; cont1u++);
}

void retardo_1ms(void) {
	uint32_t cont1m;
	for(cont1m = 0; cont1m < 20000; cont1m++);

}

void wr_data(uint8_t data) {											  // Función que escribe un dato en el LCD

	GPIO_PinWrite(gpioPORT_SSP1, CS, 0);             // Seleccionar CS = 0;

	GPIO_PinWrite(gpioPORT_SSP1, pin_A0_SSP1, 1);    // Seleccionar A0 = 1;

	SPIDrv -> Send(&data, sizeof(data));   				  // Escribir un dato (data)

	GPIO_PinWrite(gpioPORT_SSP1, CS, 1);						  // Seleccionar CS = 1;

}

void wr_cmd(uint8_t cmd) {											  // Función que escribe un comando en el LCD.

	GPIO_PinWrite(gpioPORT_SSP1, CS, 0);						  // Seleccionar CS = 0;

	GPIO_PinWrite(gpioPORT_SSP1, pin_A0_SSP1, 0);    // Seleccionar A0 = 0;

	SPIDrv -> Send(&cmd, sizeof(cmd));               // Escribir un comando (cmd)

	GPIO_PinWrite(gpioPORT_SSP1, CS, 1);	            // Seleccionar CS = 1;

}

void init(void) {

// Inicialización y configuración del driver SPI para gestionar el LCD

	SPIDrv -> Initialize(NULL);
	SPIDrv -> PowerControl(ARM_POWER_FULL);
	SPIDrv -> Control(ARM_SPI_MODE_MASTER |                //Modo master
	                  ARM_SPI_CPOL1_CPHA1 | 							 //CPOL1 y CPHA1
	                  ARM_SPI_MSB_LSB | 									 // Organización de la información MSB a LSB
	                  ARM_SPI_DATA_BITS(8), FRECUENCIA);

	//Configuracion los pines de IO que sean necesarios y programación de su valor por defecto
	//configuración como salida, puestas a nivel alto inicialmente

	PIN_Configure(gpioPORT_SSP1, pin_nRESET_SSP1, PIN_FUNC_0, PIN_PINMODE_PULLUP, PIN_PINMODE_NORMAL);
	PIN_Configure(gpioPORT_SSP1, pin_A0_SSP1, PIN_FUNC_0, PIN_PINMODE_PULLUP, PIN_PINMODE_NORMAL);
	PIN_Configure(gpioPORT_SSP1, CS, PIN_FUNC_0, PIN_PINMODE_PULLUP, PIN_PINMODE_NORMAL);

	GPIO_SetDir(gpioPORT_SSP1, pin_nRESET_SSP1, GPIO_DIR_OUTPUT);
	GPIO_SetDir(gpioPORT_SSP1, pin_A0_SSP1, GPIO_DIR_OUTPUT);
	GPIO_SetDir(gpioPORT_SSP1, CS, GPIO_DIR_OUTPUT);

	// Generar la señal de reset

	GPIO_PinWrite(gpioPORT_SSP1, pin_A0_SSP1, 0);
	GPIO_PinWrite(gpioPORT_SSP1, pin_nRESET_SSP1, 1);
	GPIO_PinWrite(gpioPORT_SSP1, CS, 1);
	GPIO_PinWrite(gpioPORT_SSP1, pin_nRESET_SSP1, 0);

	retardo_1us();

	GPIO_PinWrite(gpioPORT_SSP1, pin_nRESET_SSP1, 1);

	retardo_1ms();	// retardo de 1 ms antes de comenzar la secuencia de comandos de inicialización del LCD

}

void LCD_Uninitialize(void) {

	SPIDrv->PowerControl(ARM_POWER_OFF);  												/* Turn off SPI*/
	SPIDrv->Uninitialize();          												      /* uninitialize SPI*/
	GPIO_PinWrite(gpioPORT_SSP1, pin_nRESET_SSP1, 0);             /* Turn backlight off */

}

void LCD_Initialize(void) {

	init();
	LCD_reset();
	clear_buffer();
	copy_to_lcd(0);
	copy_to_lcd(1);
	copy_to_lcd(2);
	copy_to_lcd(3);


}


void LCD_reset(void) {

	wr_cmd(0xAE); 	// (1)  Display ON/OFF: Como D0 = 0 -> Display OFF
	wr_cmd(0xA2);	  // (11) LCD bias set: Fija el valor de la relación de la tensión de polarización del LCD. Como D0 = 0 -> 1/9 (Si D0 = 1 -> 1/7)
	wr_cmd(0xA0);	  // (8)  ADC select: Selecciona el direccionamiento de la RAM de datos del display. Como D0 = 0 -> normal (Si D0 = 1 -> revese)
	wr_cmd(0xC8); 	// (15) Common output mode select: Seleccipn el scan en las salidas COM. Como D3 = 0 -> normal (Si D3 = 1 -> revese)
	wr_cmd(0x22); 	// (17) Vo voltage regulator internal resistor ratio set: Fija la relación de resistencias interna. Como D[0-2] = 010 = 2 -> el valor quda fijado a 2 resistencias
	wr_cmd(0x2F);   // (16) Power control set: Selecciona el modo de funcionamiento de la fuente de alimentación interna. Como D[0-2] = 111 -> Power on
	wr_cmd(0x40);   // (2)  Display start address: Seleciona la linea de comienzo del display RAM. Como D[0-5] = 000000 -> El display empieza en la línea 0
	wr_cmd(0xAF);   // (1)  Display ON/OFF: Como D0 = 1 -> Display ON
	wr_cmd(0x81);   // (18) Electronic volume mode set: Contraste
	wr_cmd(0x17);   // (18) Electronic volume mode set: Valor Contraste
	wr_cmd(0xA4);   // (10) Display all points ON/OFF: Como D0 = 0 -> normal display
	wr_cmd(0xA6);   // (9)  Display normal/ reverse: Seleciona el LCD Display. Como D0 = 0 -> display normal

}


void copy_to_lcd(uint8_t pag) {
	uint8_t i;
	posicionL1 = 1;
	posicionL2 = 1;

	if(pag == 0) {
		wr_cmd(0x00);      // 4 bits de la parte baja de la dirección a 0
		wr_cmd(0x10);      // 4 bits de la parte alta de la dirección a 0
		wr_cmd(0xB0);      // Página 0

		for(i = 0; i < 128; i++) {
			wr_data(buffer[i]);
		}
	}

	else if(pag == 1) {
		wr_cmd(0x00);
		wr_cmd(0x10);
		wr_cmd(0xB1);      // Página 1

		for(i = 0; i < 128; i++) {
			wr_data(buffer[i]);
		}
	}

	else if(pag == 2) {
		wr_cmd(0x00);
		wr_cmd(0x10);
		wr_cmd(0xB2);      //Página 2
		for(i = 0; i < 128; i++) {
			wr_data(buffer[i]);
		}
	}

	else if(pag == 3) {
		wr_cmd(0x00);
		wr_cmd(0x10);
		wr_cmd(0xB3);      // Pagina 3


		for(i = 0; i < 128; i++) {
			wr_data(buffer[i]);
		}
	}

}

void EscribirFrase_L1(char linea[]) {
	uint8_t i, j, valor;
	uint16_t comienzo = 0;

	//PAGINA 0
	clear_buffer();

	for(i = 0; i < strlen(linea); i++) {
		comienzo = 25 * (linea[i] - ' ');
		for(j = 0; j < 12; j ++) {
			valor = Arial12x12[comienzo + j * 2 + 1];
			buffer[j + posicionL1] = valor;
		}
		posicionL1 = posicionL1 + Arial12x12[comienzo];
	}
	copy_to_lcd(0);

	//PAGINA 1
	clear_buffer();

	for(i = 0; i < strlen(linea); i++) {
		comienzo = 25 * (linea[i] - ' ');
		for(j = 0; j < 12; j ++) {
			valor = Arial12x12[comienzo + j * 2 + 2];
			buffer[j + posicionL1] = valor;
		}
		posicionL1 = posicionL1 + Arial12x12[comienzo];
	}
	copy_to_lcd(1);

}

void  EscribirFrase_L2(char linea[]) {
	uint8_t i, j, valor;
	uint16_t comienzo = 0;

	//PAGINA 2
	clear_buffer();

	for(i = 0; i < strlen(linea); i++) {
		comienzo = 25 * (linea[i] - ' ');
		for(j = 0; j < 12; j ++) {
			valor = Arial12x12[comienzo + j * 2 + 1];
			buffer[j + posicionL1] = valor;
		}
		posicionL1 = posicionL1 + Arial12x12[comienzo];
	}
	copy_to_lcd(2);

	//PAGINA 3
	clear_buffer();

	for(i = 0; i < strlen(linea); i++) {
		comienzo = 25 * (linea[i] - ' ');
		for(j = 0; j < 12; j ++) {
			valor = Arial12x12[comienzo + j * 2 + 2];
			buffer[j + posicionL1] = valor;
		}
		posicionL1 = posicionL1 + Arial12x12[comienzo];
	}
	copy_to_lcd(3);

}

void clear_buffer(void) {
	uint32_t i;
	for(i = 0; i < 128; i++) {
		buffer[i] = 0x00;
	}
}




///////////// PING-PONG /////////////////


/**
* @param x - Valor entre 0 y 120.
* @param y - valor entre 0 y 26.
*/
void pintarPong(uint8_t pelota_x, uint8_t pelota_y, uint8_t barra_1, uint8_t barra_2, uint8_t p1, uint8_t p2) {

	//________________PAG 0_________________
	clear_buffer();
	pintarPongPag(barra_1, 10);
	pintarPongPag(barra_2, 120);
	pintarPelotaPag(pelota_x, pelota_y);
	pintarMarcador(p1, p2);

	copy_to_lcd(0);

	//________________PAG 1_________________
	clear_buffer();
	if(barra_1 >= 8) pintarPongPag(barra_1 - 8, 10);
	if(barra_2 >= 8) pintarPongPag(barra_2 - 8, 120);
	if(pelota_y >= 8) pintarPelotaPag(pelota_x, pelota_y - 8);
	copy_to_lcd(1);

	//________________PAG 2_________________
	clear_buffer();
	if(barra_1 >= 16) pintarPongPag(barra_1 - 16, 10);
	if(barra_2 >= 16) pintarPongPag(barra_2 - 16, 120);
	if(pelota_y >= 16) pintarPelotaPag(pelota_x, pelota_y - 16);
	copy_to_lcd(2);

	//________________PAG 3_________________
	clear_buffer();
	if(barra_1 >= 24) pintarPongPag(barra_1 - 24, 10);
	if(barra_2 >= 24) pintarPongPag(barra_2 - 24, 120);
	if(pelota_y >= 24) pintarPelotaPag(pelota_x, pelota_y - 24);
	copy_to_lcd(3);

}

void pintarPongPag(uint8_t barra, uint8_t espacioLateral) {

	if(barra < 8) {
		for(uint8_t i = 0; i < 3; i++) {
			buffer [i + espacioLateral] = 0xFF >> (7 - barra);
		}
	} else if(8 <= barra && barra < 12) {
		for(uint8_t i = 0; i < 3; i++) {
			buffer [i + espacioLateral] = 0xFF;
		}
	} else if(12 <= barra && barra < 20) {
		for(uint8_t i = 0; i < 3; i++) {
			buffer [i + espacioLateral] = 0xFF << (barra - 11);
		}
	}
}

void pintarPelotaPag(uint8_t x, uint8_t y) {
	if(y < 4)
		pintarPelotaArriba(x, 3 - y);
	else if(4 <= y && y <= 10)
		pintarPelotaAbajo(x, y - 3);
}


void pintarPelotaArriba(uint8_t x, uint8_t desplazamiento) {
	buffer[x - 1] = 0x06 >> desplazamiento;
	buffer[x]   = 0x0F >> desplazamiento;
	buffer[x + 1] = 0x0F >> desplazamiento;
	buffer[x + 2] = 0x06 >> desplazamiento;
}

void pintarPelotaAbajo(uint8_t x, uint8_t desplazamiento) {
	buffer[x - 1] = 0x06 << desplazamiento;
	buffer[x]   = 0x0F << desplazamiento;
	buffer[x + 1] = 0x0F << desplazamiento;
	buffer[x + 2] = 0x06 << desplazamiento;
}

void pintarMarcador(uint8_t p1, uint8_t p2) {
	pintarNumero(p1 / 10, 50);
	pintarNumero(p1 % 10, 55);
	pintarNumero(10, 60);
	pintarNumero(p2 / 10, 65);
	pintarNumero(p2 % 10, 70);
}

void pintarNumero(uint8_t number, uint8_t pos) {
	for(uint8_t i = 0; i < 6; i++) {
		buffer[pos + i] |= num6x5[number * 5 + i];
	}
}

void pintarCuadrado(uint8_t x, uint8_t y) {
	if(y == 1) {
		buffer[x * 4] 	|= 0x0F;
		buffer[x * 4 + 1] |= 0x0F;
		buffer[x * 4 + 2] |= 0x0F;
		buffer[x * 4 + 3] |= 0x0F;
	} else {
		buffer[x * 4] 	|= 0xF0;
		buffer[x * 4 + 1] |= 0xF0;
		buffer[x * 4 + 2] |= 0xF0;
		buffer[x * 4 + 3] |= 0xF0;
	}
}


void pintarSerpiente(t_posicion mapa[], t_posicion manzanas[], uint8_t longitudSerpiente, uint8_t nManzanas) {

	clear_buffer();
	//______________PAG 0_________________
	for(uint8_t i = 0; i < longitudSerpiente; i++) {
		if(mapa[i].y == 0) pintarCuadrado(mapa[i].x, 1);
		if(mapa[i].y == 1) pintarCuadrado(mapa[i].x, 2);
	}
	for(uint8_t i = 0; i < nManzanas; i++) {
		if(manzanas[i].y == 0) pintarCuadrado(manzanas[i].x, 1);
		if(manzanas[i].y == 1) pintarCuadrado(manzanas[i].x, 2);
	}
	copy_to_lcd(0);

	clear_buffer();
	//______________PAG 1_________________
	for(uint8_t i = 0; i < longitudSerpiente; i++) {
		if(mapa[i].y == 2) pintarCuadrado(mapa[i].x, 1);
		if(mapa[i].y == 3) pintarCuadrado(mapa[i].x, 2);
	}
	for(uint8_t i = 0; i < nManzanas; i++) {
		if(manzanas[i].y == 2) pintarCuadrado(manzanas[i].x, 1);
		if(manzanas[i].y == 3) pintarCuadrado(manzanas[i].x, 2);
	}
	copy_to_lcd(1);

	clear_buffer();
	//______________PAG 2_________________
	for(uint8_t i = 0; i < longitudSerpiente; i++) {
		if(mapa[i].y == 4) pintarCuadrado(mapa[i].x, 1);
		if(mapa[i].y == 5) pintarCuadrado(mapa[i].x, 2);
	}
	for(uint8_t i = 0; i < nManzanas; i++) {
		if(manzanas[i].y == 4) pintarCuadrado(manzanas[i].x, 1);
		if(manzanas[i].y == 5) pintarCuadrado(manzanas[i].x, 2);
	}
	copy_to_lcd(2);

	clear_buffer();
	//______________PAG 3_________________
	for(uint8_t i = 0; i < longitudSerpiente; i++) {
		if(mapa[i].y == 6) pintarCuadrado(mapa[i].x, 1);
		if(mapa[i].y == 7) pintarCuadrado(mapa[i].x, 2);
	}
	for(uint8_t i = 0; i < nManzanas; i++) {
		if(manzanas[i].y == 6) pintarCuadrado(manzanas[i].x, 1);
		if(manzanas[i].y == 7) pintarCuadrado(manzanas[i].x, 2);
	}
	copy_to_lcd(3);
}


