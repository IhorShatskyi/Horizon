#include "timeErrorDetect.h"

#include "stm32f446xx.h"

// Master functions
static void Reset(void);
static void Add(float);
static float Error(void);

// Master struct
const struct timeErrorDetectFunctions TimeErrorDetect = {
	Reset,
	Add,
	Error
};

// Parameter
static float Symbol[1024];
static uint32_t Index = 0;

// Description master functions
static void Reset(void){Index = 0;}
static void Add(float src){
	Symbol[Index++] = src;
	Index &= 0x3FF;	
}
static float Error(void){
	
	uint32_t interval = Index * 3 / 4;
	
	float halfLeft = 0.f;
	float halfRight = 0.f;
	
	uint32_t shift = Index - interval;
	
	for(uint32_t i = 0; i < interval; i++){
		halfLeft += Symbol[i];
		halfRight += Symbol[i + shift];
	}
	
	if(halfLeft < 0.f)
		halfLeft *= -1.f;
	
	if(halfRight < 0.f)
		halfRight *= -1.f;
	
	Index = 0;
	
	float error = halfRight - halfLeft;
	
	return error / (float) Index;
}
