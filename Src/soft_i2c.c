#include "stm32f1xx_hal.h"
#include "soft_i2c.h"
#include "delay_us.h"

void Soft_I2C_Init(void)
{
	GPIO_InitTypeDef GPIO_Init;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE()
	;
	__HAL_RCC_GPIOD_CLK_ENABLE()
	;
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;
	Soft_I2C_SDA_High();
	Soft_I2C_SCL_High();

	GPIO_Init.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_Init.Pull = GPIO_PULLUP;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_MEDIUM;

	GPIO_Init.Pin = Soft_I2C_SDA_Pin;
	HAL_GPIO_Init(Soft_I2C_SDA_Port, &GPIO_Init);

	GPIO_Init.Pin = Soft_I2C_SCL_Pin;
	HAL_GPIO_Init(Soft_I2C_SCL_Port, &GPIO_Init);
}

uint8_t Soft_I2C_Start(void)
{
	Soft_I2C_SDA_High();
	Soft_I2C_Delay();
	Soft_I2C_SCL_High();
	Soft_I2C_Delay();
	if (!Soft_I2C_SDA_Read())
	{
		return Soft_I2C_ERR;
	}

	Soft_I2C_SDA_Low();

	Soft_I2C_Delay();
	if (Soft_I2C_SDA_Read())
	{
		return Soft_I2C_ERR;
	}

	return Soft_I2C_OK;
}

void Soft_I2C_Stop(void)
{
	Soft_I2C_SCL_Low();
	Soft_I2C_Delay();
	Soft_I2C_SDA_Low();
	Soft_I2C_Delay();
	Soft_I2C_SCL_High();
	Soft_I2C_Delay();
	Soft_I2C_SDA_High();
	Soft_I2C_Delay();
}

uint8_t Soft_I2C_Wait_ACK()
{
	uint16_t timeOut = 5000;
	Soft_I2C_SCL_Low();
	Soft_I2C_Delay();
	Soft_I2C_SDA_High();
	Soft_I2C_Delay();
	Soft_I2C_SCL_High();
	Soft_I2C_Delay();

	while (Soft_I2C_SDA_Read())
	{
		timeOut--;
		if (timeOut == 0)
		{
			Soft_I2C_Stop();
			return Soft_I2C_ERR;
		}
	}

	Soft_I2C_SCL_Low();
	Soft_I2C_Delay();

	return Soft_I2C_OK;

}

void Soft_I2C_ACK(void)
{

	Soft_I2C_SCL_Low();
	Soft_I2C_Delay();
	Soft_I2C_SDA_Low();
	Soft_I2C_Delay();
	Soft_I2C_SCL_High();
	Soft_I2C_Delay();
	Soft_I2C_SCL_Low();
	Soft_I2C_Delay();

}

void Soft_I2C_NACK(void)
{

	Soft_I2C_SCL_Low();
	Soft_I2C_Delay();
	Soft_I2C_SDA_High();
	Soft_I2C_Delay();
	Soft_I2C_SCL_High();
	Soft_I2C_Delay();
	Soft_I2C_SCL_Low();
	Soft_I2C_Delay();

}

uint8_t Soft_I2C_Send_Byte(uint8_t byte)
{

	uint8_t count = 8;

	Soft_I2C_SCL_Low();
	Soft_I2C_Delay();
	while (count--)
	{
		if (byte & 0x80)
		{
			Soft_I2C_SDA_High();
		}
		else
		{
			Soft_I2C_SDA_Low();
		}

		byte <<= 1;

		Soft_I2C_SCL_High();
		Soft_I2C_Delay();
		Soft_I2C_SCL_Low();
		Soft_I2C_Delay();
	}
	if (Soft_I2C_Wait_ACK() == Soft_I2C_ERR)
	{
		return Soft_I2C_ERR;
	}
	return Soft_I2C_OK;
}

uint8_t Soft_I2C_Receive_Byte(void)
{

	uint8_t i = 8;
	uint8_t ReceivedByte = 0;

	Soft_I2C_SDA_High();
	while (i--)
	{
		ReceivedByte <<= 1;
		Soft_I2C_SCL_Low();
		Soft_I2C_Delay();
		Soft_I2C_SCL_High();
		Soft_I2C_Delay();
		if (Soft_I2C_SDA_Read())
		{
			ReceivedByte |= 0x01;
		}
	}
	Soft_I2C_SCL_Low();
	return ReceivedByte;

}

uint8_t Soft_I2C_Write_Byte(uint8_t slaveAddr, uint8_t regAddr, uint8_t *byte)
{

	if (Soft_I2C_Start() == Soft_I2C_ERR)
	{
		Soft_I2C_Stop();
		return Soft_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(slaveAddr) == Soft_I2C_ERR)
	{
		return Soft_I2C_ERR;
	}
	if (Soft_I2C_Send_Byte(regAddr) == Soft_I2C_ERR)
	{
		return Soft_I2C_ERR;
	}

	if (byte)
	{
		if (Soft_I2C_Send_Byte(*byte) == Soft_I2C_ERR)
		{
			return Soft_I2C_ERR;
		}
	}

	Soft_I2C_Stop();

	return Soft_I2C_OK;

}

uint8_t Soft_I2C_Read_Byte(uint8_t slaveAddr, uint8_t regAddr, uint8_t *val)
{

	if (Soft_I2C_Start() == Soft_I2C_ERR)
	{
		Soft_I2C_Stop();
		return Soft_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(slaveAddr) == Soft_I2C_ERR)
	{
		return Soft_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(regAddr) == Soft_I2C_ERR)
	{
		return Soft_I2C_ERR;
	}

	if (Soft_I2C_Start() == Soft_I2C_ERR) //repeated start
	{
		return Soft_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(slaveAddr + 1) == Soft_I2C_ERR) //read bit
	{
		return Soft_I2C_ERR;
	}

	*val = Soft_I2C_Receive_Byte();
	Soft_I2C_NACK();

	Soft_I2C_Stop();

	return Soft_I2C_OK;

}

uint8_t Soft_I2C_Write_Bytes(uint8_t slaveAddr, uint8_t regAddr, uint8_t *buf,
		uint8_t num)
{

	if (Soft_I2C_Start() == Soft_I2C_ERR)
	{
		Soft_I2C_Stop();
		return Soft_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(slaveAddr) == Soft_I2C_ERR)
	{
		return Soft_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(regAddr) == Soft_I2C_ERR)
	{
		return Soft_I2C_ERR;
	}

	while (num--)
	{
		if (Soft_I2C_Send_Byte(*buf++) == Soft_I2C_ERR)
		{
			return Soft_I2C_ERR;
		}

	}

	Soft_I2C_Stop();

	return Soft_I2C_OK;

}

uint8_t Soft_I2C_Read_Bytes(uint8_t slaveAddr, uint8_t regAddr, uint8_t *buf,
		uint8_t num)
{

	if (Soft_I2C_Start() == Soft_I2C_ERR)
	{
		Soft_I2C_Stop();
		return Soft_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(slaveAddr) == Soft_I2C_ERR)
	{
		return Soft_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(regAddr) == Soft_I2C_ERR)
	{
		return Soft_I2C_ERR;
	}

	if (Soft_I2C_Start() == Soft_I2C_ERR) //repeated start
	{
		Soft_I2C_Stop();
		return Soft_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(slaveAddr + 1) == Soft_I2C_ERR) //read bit
	{
		return Soft_I2C_ERR;
	}

	while (num--)
	{
		*buf++ = Soft_I2C_Receive_Byte();

		if (num == 0)
		{
			Soft_I2C_NACK();
		}
		else
		{
			Soft_I2C_ACK();
		}
	}

	Soft_I2C_Stop();

	return Soft_I2C_OK;

}

uint8_t Soft_I2C_Scan(uint8_t slaveAddr)
{

	if (Soft_I2C_Start() == Soft_I2C_ERR)
	{
		Soft_I2C_Stop();
		return Soft_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(slaveAddr) == Soft_I2C_ERR)
	{
		return Soft_I2C_ERR;
	}
	Soft_I2C_Stop();
	return Soft_I2C_OK;
}

void Soft_I2C_Delay()
{
	//approx 275khz scl clock on 72Mhz

	uint32_t Ticks = HAL_RCC_GetSysClockFreq() / 1000000;
	while(Ticks--);


	//Delay_us(5);
}
