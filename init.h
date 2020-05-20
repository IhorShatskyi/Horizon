#ifndef INIT_H
#define INIT_H

#include "stm32f446xx.h"

extern const struct initFunctions {
	void (*const Gpio)(void);
	void (*const Systick)(void);
	void (*const Uart)(void);
	void (*const Dma)(void);
	void (*const Spi)(void);
	void (*const FlashMemory)(void);
	void (*const Rcc)(void);
	void (*const Tim)(void);
	void (*const Irq)(void);
	void (*const W5500)(void);
} Init;

#endif
