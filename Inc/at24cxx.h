#ifndef _AT24CXX_H
#define _AT24CXX_H

#include "stm32f1xx_hal.h"


/******************at24cxx config******************************/
#define AT24CXX_EEPROM_ADDR      0XA6

#define AT24CXX_ADDRESS_WIDTH    8 // 8 or 16
#define AT24CXX_PAGE_LENGTH      8 // 8 , 16 , 32 or 64

/******************at24cxx config******************************/

#define AT24CXX_OK		0
#define AT24CXX_ERR		1


void    AT24CXX_Init();
uint8_t AT24CXX_Write_Byte(uint16_t Address, uint8_t data);
uint8_t AT24CXX_Read_Byte(uint16_t Address, uint8_t* data);
uint8_t AT24CXX_Write_Page(uint16_t Address, uint8_t *buf, uint16_t len);
uint8_t AT24CXX_Read_Buffer(uint16_t Address, uint8_t *buf, uint16_t len);
uint8_t AT24CXX_Write_Buffer(uint16_t Address, uint8_t *buf, uint16_t len);





#endif 
