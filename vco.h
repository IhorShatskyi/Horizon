#ifndef VCO_H
#define VCO_H

#include "iq.h"

extern const struct vcoFunctions {
	void (*const Update)(complex *, complex *);
} Vco;

#endif
