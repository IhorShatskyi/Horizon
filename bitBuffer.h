#ifndef BIT_BUFFER_H
#define BIT_BUFFER_H

#include "stm32f446xx.h"

extern const struct bitBufferFunctions {
	
	struct {
		void (*const Add)(uint8_t);
		uint8_t (*const Get)(void);
	} Bit;

} BitBuffer;

#endif
