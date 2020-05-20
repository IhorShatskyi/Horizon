#ifndef STORONA_DRIVER_H
#define STORONA_DRIVER_H

#include "stm32f446xx.h"
#include "iq.h"
#include "list.h"

extern list listStoronaDriver;

extern const struct storonaDriverFunctions {

	void (*const Init)(void);	
	void (*const Ethernet)(uint32_t, uint32_t, uint16_t);
	void (*const Bit)(uint8_t);
	void (*const Percent)(uint8_t);

} StoronaDriver;

#endif
