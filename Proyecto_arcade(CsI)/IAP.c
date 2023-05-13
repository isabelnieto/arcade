
#include "IAP.h"

/** The area will be erase and program */
#define FLASH_PROG_AREA_START       0x30000 // Direccion de memoria en la que empieza
#define FLASH_PROG_AREA_SIZE				0x8000 // Tamaño del sector

uint8_t __attribute__((aligned(4))) ArrayFlash[11];

uint8_t *ptr;

void c_entry(void) {

	//int i;
	uint32_t flash_prog_area_sec_start;
	uint32_t flash_prog_area_sec_end;

	flash_prog_area_sec_start = 	GetSecNum(FLASH_PROG_AREA_START);
	flash_prog_area_sec_end 	=   GetSecNum(FLASH_PROG_AREA_START + FLASH_PROG_AREA_SIZE);


	ptr = (uint8_t*)(FLASH_PROG_AREA_START + 10);
	ArrayFlash[10] = *ptr;

	ptr = (uint8_t*)(FLASH_PROG_AREA_START);

	//Borro
	EraseSector(flash_prog_area_sec_start, flash_prog_area_sec_end);

	CopyRAM2Flash(ptr, ArrayFlash, IAP_WRITE_256); // Copia de la RAM a la FLASH

}



