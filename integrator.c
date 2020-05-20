#include "integrator.h"

// Master functions
static complex Update(integrator *, complex);

// Master struct
const struct integratorFunctions Integrator = {
	Update
};

// Description master functions
complex Update(integrator *integrator, complex sempl){
	
	uint32_t index = integrator->index;
	
	index++;
	index &= 0x3FF;
	
	integrator->lineDelay[index] = sempl;
	
	integrator->index = index;
	
	integrator->accumulation.re += sempl.re;
	integrator->accumulation.im += sempl.im;
	
	index -= integrator->interval;
	index &= 0x3FF;
	
	complex oldSempl = integrator->lineDelay[index];
	
	integrator->accumulation.re -= oldSempl.re;
	integrator->accumulation.im -= oldSempl.im;	
	
	complex result;
	result.re = integrator->accumulation.re / (float)(integrator->interval);
	result.im = integrator->accumulation.im / (float)(integrator->interval);
	
	return result;
}
