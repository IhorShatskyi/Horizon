#include "loopFilter.h"

#define KP 0.0001f
#define KI 0.000001f

#define UP_THRESHOLD 0.00002f
#define DOWN_THRESHOLD -0.00002f

// Master functions
static float Update(float);

// Master struct
const struct loopFilterFunctions LoopFilter = {
	Update
};

// Description master functions
static float Update(float src){
	
	static float integrator = 0.f;
	
	float error = integrator + src * KP;

	if(error > UP_THRESHOLD)
		error = UP_THRESHOLD;
	
	if(error < DOWN_THRESHOLD)
		error = DOWN_THRESHOLD;	

	
	integrator += src * KI;

	if(integrator > UP_THRESHOLD)
		integrator = UP_THRESHOLD;
	
	if(integrator < DOWN_THRESHOLD)
		integrator = DOWN_THRESHOLD;	
		
	return error;
}
