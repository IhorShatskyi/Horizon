#ifndef DEMODULATOR_QFSK_H
#define DEMODULATOR_QFSK_H

#include "stm32f446xx.h"
#include "iq.h"

extern const struct demodulatorQfskFunctions {
	
	struct {
		void (*const Set)(float);
		float (*const Get)(void);
	} RelativeBaudRate;
	
	struct {
		void (*const Set)(float);
		float (*const Get)(void);
	} RelativeFrequencyShift;	
	
	void (*const AddSempl)(complex);
	
	struct{
		void (*const Set)(uint8_t);
	} Combination;
	
	struct{
		void (*const Set)(uint8_t);
	} Inversion;
	
	void(*const Reset)(void);
	
} DemodulatorQFSK;

#endif

