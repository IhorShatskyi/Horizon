#include "spi.h"

static void Write(uint8_t);
static uint8_t Read(void);

const struct spiFunctions Spi = {
	Write,
	Read
};

static void Write(uint8_t data){
	static uint32_t temp;
	*(volatile uint8_t*)&(SPI1->DR) = data;
	while (!(SPI1->SR & SPI_SR_RXNE));
	temp = SPI1->DR;
}
static uint8_t Read(void){
	*(volatile uint8_t *)&(SPI1->DR) = 0x55;	
	while (!(SPI1->SR & SPI_SR_RXNE));
	return (uint8_t)(SPI1->DR);
}
