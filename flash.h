#ifndef FLASH_H
#define FLASH_H

#include "stm32f446xx.h"

extern const struct flashFunction {
	void (*const Write)(uint32_t, uint32_t);
	uint32_t (*const Read)(uint32_t);
	void (*const Lock)(void);
	void (*const Unlock)(void);
	void (*const EraseSector)(uint32_t);
} Flash;

#endif
