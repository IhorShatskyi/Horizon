#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "stm32f446xx.h"
#include "iq.h"

typedef struct {
	uint32_t interval;
	complex accumulation;
	uint32_t index;
	complex lineDelay[1024];
} integrator;

extern const struct integratorFunctions {
	complex (*const Update)(integrator *, complex);
} Integrator;

#endif
