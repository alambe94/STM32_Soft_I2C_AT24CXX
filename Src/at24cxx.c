#include "at24cxx.h"
#include "soft_i2c.h"

void AT24CXX_Init()
{
	Soft_I2C_Init();
}

uint8_t AT24CXX_Write_Byte(uint16_t Address, uint8_t data)
{
	if (Soft_I2C_Start() == Soft_I2C_ERR)/*generate star condition*/
	{
		Soft_I2C_Stop();
		return AT24CXX_ERR;
	}

	if (Soft_I2C_Send_Byte(AT24CXX_EEPROM_ADDR + 0) == Soft_I2C_ERR)/*Send device address + write bit */
	{
		return AT24CXX_ERR;
	}

	if (AT24CXX_ADDRESS_WIDTH > ADDRESS_WIDTH_8)/* Only if address is 16 bit */
	{
		if (Soft_I2C_Send_Byte((Address >> 8)) == Soft_I2C_ERR)
		{
			return AT24CXX_ERR;
		}
	}
	if (Soft_I2C_Send_Byte(Address) == Soft_I2C_ERR)/*send register address to write*/
	{
		return AT24CXX_ERR;
	}

	if (Soft_I2C_Send_Byte(data) == Soft_I2C_ERR)
	{
		return AT24CXX_ERR;
	}
	Soft_I2C_Stop();
	HAL_Delay(5);/*Memory Programming Time approx 5ms*//*3ms for BL24CXX */
	return AT24CXX_OK;
}

uint8_t AT24CXX_Read_Byte(uint16_t Address, uint8_t* data)
{

	if (Soft_I2C_Start() == Soft_I2C_ERR)/*generate star condition*/
	{
		Soft_I2C_Stop();
		return AT24CXX_ERR;
	}

	if (Soft_I2C_Send_Byte(AT24CXX_EEPROM_ADDR + 0) == Soft_I2C_ERR)/*Send device address + write bit */
	{
		return AT24CXX_ERR;
	}

	if (AT24CXX_ADDRESS_WIDTH > ADDRESS_WIDTH_8)/* Only if address is 16 bit */
	{
		if (Soft_I2C_Send_Byte((Address >> 8)) == Soft_I2C_ERR)
		{
			return AT24CXX_ERR;
		}
	}
	if (Soft_I2C_Send_Byte(Address) == Soft_I2C_ERR)/*send register address to write*/
	{
		return AT24CXX_ERR;
	}

	if (Soft_I2C_Start() == Soft_I2C_ERR) //repeated start
	{
		Soft_I2C_Stop();
		return AT24CXX_ERR;
	}

	if (Soft_I2C_Send_Byte(AT24CXX_EEPROM_ADDR + 1) == Soft_I2C_ERR)/*Send device address + read bit */
	{
		return AT24CXX_ERR;
	}

	*data = Soft_I2C_Receive_Byte();

	Soft_I2C_Stop();/*Explicit stop condition for single byte*/
	return AT24CXX_OK;
}

uint8_t AT24CXX_Write_Page(uint16_t Address, uint8_t *buf, uint16_t len)
{

	if (len == 0u)
	{
		return AT24CXX_OK;
	}

	if (Soft_I2C_Start() == Soft_I2C_ERR)/*generate star condition*/
	{
		Soft_I2C_Stop();
		return AT24CXX_ERR;
	}

	if (Soft_I2C_Send_Byte(AT24CXX_EEPROM_ADDR + 0) == Soft_I2C_ERR)/*Send device address + write bit */
	{
		return AT24CXX_ERR;
	}

	if (AT24CXX_ADDRESS_WIDTH > ADDRESS_WIDTH_8)/* Only if address is 16 bit */
	{
		if (Soft_I2C_Send_Byte((Address >> 8)) == Soft_I2C_ERR)
		{
			return AT24CXX_ERR;
		}
	}
	if (Soft_I2C_Send_Byte(Address) == Soft_I2C_ERR)/*send register address to write*/
	{
		return AT24CXX_ERR;
	}

	while (len--)
	{
		if (Soft_I2C_Send_Byte(*buf++) == Soft_I2C_ERR)/*write data to address*/
		{
			return AT24CXX_ERR;
		}
	}
	Soft_I2C_Stop();
	HAL_Delay(5);/*Memory Programming Time approx 5ms*//*3ms for BL24CXX */
	return AT24CXX_OK;
}

uint8_t AT24CXX_Read_Buffer(uint16_t Address, uint8_t *buf, uint16_t len)
{

	if (Soft_I2C_Start() == Soft_I2C_ERR)/*generate star condition*/
	{
		Soft_I2C_Stop();
		return AT24CXX_ERR;
	}

	if (Soft_I2C_Send_Byte(AT24CXX_EEPROM_ADDR + 0) == Soft_I2C_ERR)/*Send device address + write bit */
	{
		return AT24CXX_ERR;
	}

	if (AT24CXX_ADDRESS_WIDTH > ADDRESS_WIDTH_8)/* Only if address is 16 bit */
	{
		if (Soft_I2C_Send_Byte((Address >> 8)) == Soft_I2C_ERR)
		{
			return AT24CXX_ERR;
		}
	}
	if (Soft_I2C_Send_Byte(Address) == Soft_I2C_ERR)/*send register address to write*/
	{
		return AT24CXX_ERR;
	}

	if (Soft_I2C_Start() == Soft_I2C_ERR) //repeated start
	{
		Soft_I2C_Stop();
		return AT24CXX_ERR;
	}

	if (Soft_I2C_Send_Byte(AT24CXX_EEPROM_ADDR + 1) == Soft_I2C_ERR) //read bit
	{
		return AT24CXX_ERR;
	}

	while (len--)
	{
		*buf++ = Soft_I2C_Receive_Byte();

		if (len == 0)
		{
			Soft_I2C_NACK();
		}
		else
		{
			Soft_I2C_ACK();
		}
	}

	Soft_I2C_Stop();

	return AT24CXX_OK;

}

/*this function can write any number of bytes in arbitary location*/
uint8_t AT24CXX_Write_Buffer(uint16_t Address, uint8_t *buf, uint16_t len)
{
	uint16_t index;
	uint16_t page_counter, byte_counter;

	byte_counter = (AT24CXX_PAGE_LENGTH - (Address % AT24CXX_PAGE_LENGTH));
	if (len < byte_counter)
	{
		byte_counter = len;
	}

	if (AT24CXX_Write_Page(Address, buf, byte_counter) == AT24CXX_ERR)
	{
		return AT24CXX_ERR;
	}

	page_counter = ((len - byte_counter) / AT24CXX_PAGE_LENGTH);

	for (index = 0; index < (page_counter); index++)
	{
		if (AT24CXX_Write_Page(
				(Address + byte_counter + (index * AT24CXX_PAGE_LENGTH)),
				(buf + byte_counter + (index * AT24CXX_PAGE_LENGTH)),
				AT24CXX_PAGE_LENGTH) == AT24CXX_ERR)
		{
			return AT24CXX_ERR;
		}
	}

	if (AT24CXX_Write_Page(
			(Address + byte_counter + (page_counter * AT24CXX_PAGE_LENGTH)),
			(buf + byte_counter + (page_counter * AT24CXX_PAGE_LENGTH)),
			(len - byte_counter - (page_counter * AT24CXX_PAGE_LENGTH))) == AT24CXX_ERR)
	{
		return AT24CXX_ERR;
	}
	return AT24CXX_OK;
}
