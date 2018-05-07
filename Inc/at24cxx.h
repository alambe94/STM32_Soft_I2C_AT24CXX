#ifndef _AT24CXX_H
#define _AT24CXX_H

#include "stm32f1xx_hal.h"




#define AT24CXX_OK		0
#define AT24CXX_ERR		1

#define ADDRESS_WIDTH_16 16u
#define ADDRESS_WIDTH_8  8u

#define PAGE_LENGTH_8   8u
#define PAGE_LENGTH_16  16u
#define PAGE_LENGTH_64  64u


/******************at24cxx config******************************/
#define AT24CXX_EEPROM_ADDR      (uint8_t)0XA6

#define AT24CXX_ADDRESS_WIDTH    ADDRESS_WIDTH_8
#define AT24CXX_PAGE_LENGTH      PAGE_LENGTH_8


void    AT24CXX_Init();
uint8_t AT24CXX_Write_Byte(uint16_t Address, uint8_t data);
uint8_t AT24CXX_Read_Byte(uint16_t Address, uint8_t* data);
uint8_t AT24CXX_Write_Page(uint16_t Address, uint8_t *buf, uint16_t len);
uint8_t AT24CXX_Read_Buffer(uint16_t Address, uint8_t *buf, uint16_t len);
uint8_t AT24CXX_Write_Buffer(uint16_t Address, uint8_t *buf, uint16_t len);
















#endif 
