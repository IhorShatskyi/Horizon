#ifndef MODULATOR_QFSK_H
#define MODULATOR_QFSK_H

#include "stm32f446xx.h"
#include "iq.h"

extern const struct modulatorQfskFunctions {
	
	struct {
		void (*const Set)(float);
		float (*const Get)(void);
	} RelativeBaudRate;

	struct {
		void (*const Set)(float);
		float (*const Get)(void);
	} RelativeFrequencyShift;	
	
	complex (*const GetSempl)(void);
	
	struct{
		void (*const Set)(uint8_t);
	} Combination;
	
	void (*const Reset)(void);
	
} ModulatorQFSK;

#endif

