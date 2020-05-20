#include "vco.h"

// Master functions
static void Update(complex *, complex *);

// Master struct
const struct vcoFunctions Vco = {
	Update
};

// Description master functions
static void Update(complex *accum, complex *phase){
	float gain = 1.96f - Complex.AbsInSquare(*accum);
	accum->re *= gain;
	accum->im *= gain;
	*accum = Complex.Multiply(*accum, *phase);
}
