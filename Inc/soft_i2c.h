/*
* Soft_I2C.h
*
*  Created on: 10-Jul-2017
*      Author: medprime
*/

#include "stm32f1xx_hal.h"

#ifndef SOFT_I2C_H_
#define SOFT_I2C_H_

#define SOFT_I2C_OK		    0
#define SOFT_I2C_ERR		1

#ifndef _SOFT_I2C_SDA_PIN
#error "Please define _SOFT_I2C_SDA_PIN as MCU GPIO_PIN in main.h"
#endif

#ifndef _SOFT_I2C_SDA_PORT
#error "Please define _SOFT_I2C_SDA_PORT as MCU GPIO_PORT  in main.h"
#endif

#ifndef _SOFT_I2C_SCL_PIN
#error "Please define_SOFT_I2C_SCL_PIN as MCU GPIO_PIN  in main.h"
#endif

#ifndef _SOFT_I2C_SCL_PORT
#error "Please define _SOFT_I2C_SCL_PORT as MCU GPIO_PORT  in main.h"
#endif

#define SOFT_I2C_SDA_PIN		_SOFT_I2C_SDA_PIN
#define SOFT_I2C_SDA_PORT		_SOFT_I2C_SDA_PORT


#define SOFT_I2C_SCL_PIN		_SOFT_I2C_SCL_PIN
#define SOFT_I2C_SCL_PORT		_SOFT_I2C_SCL_PORT

//SDA		
//SCL		
#define SOFT_I2C_SDA_HIGH()	    HAL_GPIO_WritePin(SOFT_I2C_SDA_PORT,SOFT_I2C_SDA_PIN,GPIO_PIN_SET)
#define SOFT_I2C_SDA_LOW()	    HAL_GPIO_WritePin(SOFT_I2C_SDA_PORT,SOFT_I2C_SDA_PIN,GPIO_PIN_RESET)
#define SOFT_I2C_SDA_READ()	    HAL_GPIO_ReadPin(SOFT_I2C_SDA_PORT,SOFT_I2C_SDA_PIN)


#define SOFT_I2C_SCL_HIGH()	    HAL_GPIO_WritePin(SOFT_I2C_SCL_PORT,SOFT_I2C_SCL_PIN,GPIO_PIN_SET)
#define SOFT_I2C_SCL_LOW()	    HAL_GPIO_WritePin(SOFT_I2C_SCL_PORT,SOFT_I2C_SCL_PIN,GPIO_PIN_RESET)
#define SOFT_I2C_SCL_READ()	    HAL_GPIO_ReadPin(SOFT_I2C_SCL_PORT,SOFT_I2C_SCL_PIN)



void Soft_I2C_Init(void);

uint8_t Soft_I2C_Write_Byte(uint8_t slave_address, uint8_t register_address,
                           uint8_t *byte);

uint8_t Soft_I2C_Read_Byte(uint8_t slave_address, uint8_t register_address,
                          uint8_t *val);

uint8_t Soft_I2C_Write_Bytes(uint8_t slave_address, uint8_t register_address,
                            uint8_t *buf, uint8_t num);

uint8_t Soft_I2C_Read_Bytes(uint8_t slave_address, uint8_t register_address,
                           uint8_t *buf, uint8_t num);

uint8_t Soft_I2C_Start(void);

void Soft_I2C_Stop(void);

uint8_t Soft_I2C_Wait_ACK();

void Soft_I2C_ACK(void);

void Soft_I2C_NACK(void);

uint8_t Soft_I2C_Send_Byte(uint8_t byte);

uint8_t Soft_I2C_Scan(uint8_t slave_address);

void Soft_I2C_Delay();


uint8_t Soft_I2C_Receive_Byte(void);

#endif /* SOFT_I2C_H_ */
