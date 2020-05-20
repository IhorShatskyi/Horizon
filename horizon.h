#ifndef HORIZON_H
#define HORIZON_H

#include "stm32f446xx.h"

extern const struct horizonFunctions {
	
	void (*const Init)(void);
	void (*const Run)(void);
	void (*const Stop)(void);
	
} Horizon;

#endif
