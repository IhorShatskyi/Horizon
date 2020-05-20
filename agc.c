#include "agc.h"
#include "stm32f446xx.h"
#include "math.h"
#include "print.h"

#define ALPHA 0.01f
#define LOG_R 0.f
#define UP_GAIN_THRESHOLD 40 // dB
#define DOWN_GAIN_THRESHOLD -20 // dB
#define WINDOW 32

// Master functions
static void Update(float);
static void Reset(void);
static float Gain(void);

// Master struct
const struct agcFunctions Agc = {
	Update,
	Reset,
	Gain
};

// Parameters
static float integrator = 0.f;
static float lineDelay[WINDOW];
static float accum = 0.f;
static uint32_t index = 0;

// Description master functions
static void Update(float level){
	
	index++;
	index %= WINDOW;
	accum += level - lineDelay[index];	
	lineDelay[index] = level;
	
	integrator += ALPHA * ((float)LOG_R - log10f(accum / (float)WINDOW));

	if(integrator > (float)UP_GAIN_THRESHOLD / 10.f)
		integrator = (float)UP_GAIN_THRESHOLD / 10.f;
	if(integrator < (float)DOWN_GAIN_THRESHOLD / 10.f)
		integrator = (float)DOWN_GAIN_THRESHOLD / 10.f;

}

static void Reset(void){
	integrator = 0.f;
	for(uint32_t i = 0; i < WINDOW; i++)
		lineDelay[i] = 0.f;
	accum = 0.f;
	index = 0;
}

static float Gain(void){return powf(integrator, 10.f);}
