#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "Driver_SPI.h"
#include "PIN_LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include <stdio.h>
#include <string.h>
#include "LPC17xx.h"

#define FRECUENCIA 20000000    //  Frecuencia del SCLK, 20MHz 
#define segunda 256 // Inicio pagina 2 (mitad)

#define gpioPORT_SSP1    0
#define pin_nRESET_SSP1  8
#define pin_A0_SSP1      6
#define CS               18

extern ARM_DRIVER_SPI Driver_SPI1;        // Driver que llamamos SPI1 y esta definido de manera externa
static ARM_DRIVER_SPI* SPIDrv = &Driver_SPI1;    // el driver apunta al contenido del driver

//funciones del lcd
void retardo_1us(void);
void retardo_1ms(void);
void wr_data(uint8_t data);
void wr_cmd(uint8_t cmd);

void init(void);
void LCD_Uninitialize(void);
void LCD_Initialize(void);
void LCD_reset(void);
void copy_to_lcd(uint8_t pag);

void EscribirFrase_L1(char linea[]);
void EscribirFrase_L2(char linea[]);

void clear_buffer(void);


////////////// PING-PONG ///////////////

void pintarCuadrado(uint8_t lado, uint8_t pos);
void escribe_letra(char letra);
void pintarPongPag(uint8_t barra, uint8_t espacioLateral);
void pintarPelotaPag(uint8_t x, uint8_t y);
void pintarPelotaArriba(uint8_t x, uint8_t desplazamiento);
void pintarPelotaAbajo(uint8_t x, uint8_t desplazamiento);
void pintarPong(uint8_t pelota_x, uint8_t pelota_y, uint8_t barra_1, uint8_t barra_2, uint8_t p1, uint8_t p2);
void pintarMarcador(uint8_t p1, uint8_t p2);
void pintarNumero(uint8_t number, uint8_t pos);


////////////// SNAKE //////////////
typedef struct {
	uint8_t x;
	uint8_t y;
} t_posicion;
void pintarSerpiente(t_posicion mapa[], t_posicion manzanas[], uint8_t longitudSerpiente, uint8_t nManzanas);


#endif

