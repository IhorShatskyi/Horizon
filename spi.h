#ifndef SPI_H
#define SPI_H

#include "stm32f446xx.h"

extern const struct spiFunctions{
	void (*const Write)(uint8_t);
	uint8_t (*const Read)(void);
} Spi;

#endif
