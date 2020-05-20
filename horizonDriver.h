#ifndef HORIZON_DRIVER_H
#define HORIZON_DRIVER_H

#include "stm32f446xx.h"
#include "iq.h"
#include "list.h"

extern list listHorizonDriver;

extern const struct horizonDriverFunctions {

	struct {
		void (*const Mode)(uint32_t);
		void (*const Width)(uint32_t);
		void (*const Frequency)(uint32_t);
		void (*const Sempl)(complex);
	} Ddc;
	
	struct {
		void (*const Mode)(uint32_t);
		void (*const Width)(uint32_t);
		void (*const Frequency)(uint32_t);
		void (*const Percent)(uint32_t);		
	} Duc;

	void (*const Ethernet)(uint32_t, uint32_t, uint32_t, uint32_t);
	void (*const Init)(char *);
	void (*const Error)(uint32_t);

} HorizonDriver;

#endif
