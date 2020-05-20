#ifndef STORONA_H
#define STORONA_H

#include "stm32f446xx.h"

#define TRANSFER_BANDWIDTH 12000
#define RECEIVER_BANDWIDTH 12000

extern const struct storonaFunctions {
	
	void (*const Init)(void);
	void (*const Run)(void);
	void (*const Stop)(void);
	
} Storona;

#endif
