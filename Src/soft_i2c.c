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
	SOFT_I2C_SDA_HIGH();
	SOFT_I2C_SCL_HIGH();

	GPIO_Init.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_Init.Pull = GPIO_PULLUP;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_MEDIUM;

	GPIO_Init.Pin = SOFT_I2C_SDA_PIN;
	HAL_GPIO_Init(SOFT_I2C_SDA_PORT, &GPIO_Init);

	GPIO_Init.Pin = SOFT_I2C_SCL_PIN;
	HAL_GPIO_Init(SOFT_I2C_SCL_PORT, &GPIO_Init);
}


void Soft_I2C_Delay()
{
	//approx 275khz scl clock on 72Mhz

	uint32_t Ticks = HAL_RCC_GetSysClockFreq() / 1000000;
	while(Ticks--);

	//Delay_us(5);

}


uint8_t Soft_I2C_Start(void)
{
	SOFT_I2C_SDA_HIGH();
	Soft_I2C_Delay();
	SOFT_I2C_SCL_HIGH();
	Soft_I2C_Delay();
	if (!SOFT_I2C_SDA_READ())
	{
		return SOFT_I2C_ERR;
	}

	SOFT_I2C_SDA_LOW();

	Soft_I2C_Delay();
	if (SOFT_I2C_SDA_READ())
	{
		return SOFT_I2C_ERR;
	}

	return SOFT_I2C_OK;
}

void Soft_I2C_Stop(void)
{
	SOFT_I2C_SCL_LOW();
	Soft_I2C_Delay();
	SOFT_I2C_SDA_LOW();
	Soft_I2C_Delay();
	SOFT_I2C_SCL_HIGH();
	Soft_I2C_Delay();
	SOFT_I2C_SDA_HIGH();
	Soft_I2C_Delay();
}

uint8_t Soft_I2C_Wait_ACK()
{
	uint16_t timeOut = 5000;
	SOFT_I2C_SCL_LOW();
	Soft_I2C_Delay();
	SOFT_I2C_SDA_HIGH();
	Soft_I2C_Delay();
	SOFT_I2C_SCL_HIGH();
	Soft_I2C_Delay();

	while (SOFT_I2C_SDA_READ())
	{
		timeOut--;
		if (timeOut == 0)
		{
			Soft_I2C_Stop();
			return SOFT_I2C_ERR;
		}
	}

	SOFT_I2C_SCL_LOW();
	Soft_I2C_Delay();

	return SOFT_I2C_OK;

}

void Soft_I2C_ACK(void)
{

	SOFT_I2C_SCL_LOW();
	Soft_I2C_Delay();
	SOFT_I2C_SDA_LOW();
	Soft_I2C_Delay();
	SOFT_I2C_SCL_HIGH();
	Soft_I2C_Delay();
	SOFT_I2C_SCL_LOW();
	Soft_I2C_Delay();

}

void Soft_I2C_NACK(void)
{

	SOFT_I2C_SCL_LOW();
	Soft_I2C_Delay();
	SOFT_I2C_SDA_HIGH();
	Soft_I2C_Delay();
	SOFT_I2C_SCL_HIGH();
	Soft_I2C_Delay();
	SOFT_I2C_SCL_LOW();
	Soft_I2C_Delay();

}

uint8_t Soft_I2C_Send_Byte(uint8_t byte)
{

	uint8_t count = 8;

	SOFT_I2C_SCL_LOW();
	Soft_I2C_Delay();
	while (count--)
	{
		if (byte & 0x80)
		{
			SOFT_I2C_SDA_HIGH();
		}
		else
		{
			SOFT_I2C_SDA_LOW();
		}

		byte <<= 1;

		SOFT_I2C_SCL_HIGH();
		Soft_I2C_Delay();
		SOFT_I2C_SCL_LOW();
		Soft_I2C_Delay();
	}
	if (Soft_I2C_Wait_ACK() == SOFT_I2C_ERR)
	{
		return SOFT_I2C_ERR;
	}
	return SOFT_I2C_OK;
}

uint8_t Soft_I2C_Receive_Byte(void)
{

	uint8_t i = 8;
	uint8_t ReceivedByte = 0;

	SOFT_I2C_SDA_HIGH();
	while (i--)
	{
		ReceivedByte <<= 1;
		SOFT_I2C_SCL_LOW();
		Soft_I2C_Delay();
		SOFT_I2C_SCL_HIGH();
		Soft_I2C_Delay();
		if (SOFT_I2C_SDA_READ())
		{
			ReceivedByte |= 0x01;
		}
	}
	SOFT_I2C_SCL_LOW();
	return ReceivedByte;

}

uint8_t Soft_I2C_Write_Byte(uint8_t slave_address, uint8_t register_address, uint8_t *byte)
{

	if (Soft_I2C_Start() == SOFT_I2C_ERR)
	{
		Soft_I2C_Stop();
		return SOFT_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(slave_address) == SOFT_I2C_ERR)
	{
		return SOFT_I2C_ERR;
	}
	if (Soft_I2C_Send_Byte(register_address) == SOFT_I2C_ERR)
	{
		return SOFT_I2C_ERR;
	}

	if (byte)
	{
		if (Soft_I2C_Send_Byte(*byte) == SOFT_I2C_ERR)
		{
			return SOFT_I2C_ERR;
		}
	}

	Soft_I2C_Stop();

	return SOFT_I2C_OK;

}

uint8_t Soft_I2C_Read_Byte(uint8_t slave_address, uint8_t register_address, uint8_t *val)
{

	if (Soft_I2C_Start() == SOFT_I2C_ERR)
	{
		Soft_I2C_Stop();
		return SOFT_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(slave_address) == SOFT_I2C_ERR)
	{
		return SOFT_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(register_address) == SOFT_I2C_ERR)
	{
		return SOFT_I2C_ERR;
	}

	if (Soft_I2C_Start() == SOFT_I2C_ERR) //repeated start
	{
		return SOFT_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(slave_address + 1) == SOFT_I2C_ERR) //read bit
	{
		return SOFT_I2C_ERR;
	}

	*val = Soft_I2C_Receive_Byte();
	Soft_I2C_NACK();

	Soft_I2C_Stop();

	return SOFT_I2C_OK;

}

uint8_t Soft_I2C_Write_Bytes(uint8_t slave_address, uint8_t register_address, uint8_t *buf,
		uint8_t num)
{

	if (Soft_I2C_Start() == SOFT_I2C_ERR)
	{
		Soft_I2C_Stop();
		return SOFT_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(slave_address) == SOFT_I2C_ERR)
	{
		return SOFT_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(register_address) == SOFT_I2C_ERR)
	{
		return SOFT_I2C_ERR;
	}

	while (num--)
	{
		if (Soft_I2C_Send_Byte(*buf++) == SOFT_I2C_ERR)
		{
			return SOFT_I2C_ERR;
		}

	}

	Soft_I2C_Stop();

	return SOFT_I2C_OK;

}

uint8_t Soft_I2C_Read_Bytes(uint8_t slave_address, uint8_t register_address, uint8_t *buf,
		uint8_t num)
{

	if (Soft_I2C_Start() == SOFT_I2C_ERR)
	{
		Soft_I2C_Stop();
		return SOFT_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(slave_address) == SOFT_I2C_ERR)
	{
		return SOFT_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(register_address) == SOFT_I2C_ERR)
	{
		return SOFT_I2C_ERR;
	}

	if (Soft_I2C_Start() == SOFT_I2C_ERR) //repeated start
	{
		Soft_I2C_Stop();
		return SOFT_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(slave_address + 1) == SOFT_I2C_ERR) //read bit
	{
		return SOFT_I2C_ERR;
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

	return SOFT_I2C_OK;

}

uint8_t Soft_I2C_Scan(uint8_t slave_address)
{

	if (Soft_I2C_Start() == SOFT_I2C_ERR)
	{
		Soft_I2C_Stop();
		return SOFT_I2C_ERR;
	}

	if (Soft_I2C_Send_Byte(slave_address) == SOFT_I2C_ERR)
	{
		return SOFT_I2C_ERR;
	}
	Soft_I2C_Stop();
	return SOFT_I2C_OK;
}
