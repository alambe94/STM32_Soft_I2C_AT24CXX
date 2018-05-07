/*
* Soft_I2C.h
*
*  Created on: 10-Jul-2017
*      Author: medprime
*/

#include "stm32f1xx_hal.h"

#ifndef SOFT_I2C_H_
#define SOFT_I2C_H_

#define Soft_I2C_OK		    0
#define Soft_I2C_ERR		1

#define Soft_I2C_SDA_Pin		GPIO_PIN_7
#define Soft_I2C_SDA_Port		GPIOB


#define Soft_I2C_SCL_Pin		GPIO_PIN_6
#define Soft_I2C_SCL_Port		GPIOB

//SDA		
//SCL		
#define SDA_High()	HAL_GPIO_WritePin(Soft_I2C_SDA_Port,Soft_I2C_SDA_Pin,GPIO_PIN_SET)
#define SDA_Low()	HAL_GPIO_WritePin(Soft_I2C_SDA_Port,Soft_I2C_SDA_Pin,GPIO_PIN_RESET)
#define SDA_Read()	HAL_GPIO_ReadPin(Soft_I2C_SDA_Port,Soft_I2C_SDA_Pin)


#define SCL_High()	HAL_GPIO_WritePin(Soft_I2C_SCL_Port,Soft_I2C_SCL_Pin,GPIO_PIN_SET)
#define SCL_Low()	HAL_GPIO_WritePin(Soft_I2C_SCL_Port,Soft_I2C_SCL_Pin,GPIO_PIN_RESET)
#define SCL_Read()	HAL_GPIO_ReadPin(Soft_I2C_SCL_Port,Soft_I2C_SCL_Pin)



void Soft_I2C_Init(void);

uint8_t Soft_I2C_Write_Byte(uint8_t slaveAddr, uint8_t regAddr,
                           uint8_t *byte);

uint8_t Soft_I2C_Read_Byte(uint8_t slaveAddr, uint8_t regAddr,
                          uint8_t *val);

uint8_t Soft_I2C_Write_Bytes(uint8_t slaveAddr, uint8_t regAddr,
                            uint8_t *buf, uint8_t num);

uint8_t Soft_I2C_Read_Bytes(uint8_t slaveAddr, uint8_t regAddr,
                           uint8_t *buf, uint8_t num);

uint8_t Soft_I2C_Start(void);

void Soft_I2C_Stop(void);

uint8_t Soft_I2C_Wait_ACK();

void Soft_I2C_ACK(void);

void Soft_I2C_NACK(void);

uint8_t Soft_I2C_Send_Byte(uint8_t byte);

uint8_t Soft_I2C_Scan(uint8_t slaveAddr);

void Soft_I2C_Delay();


uint8_t Soft_I2C_Receive_Byte(void);

#endif /* SOFT_I2C_H_ */
