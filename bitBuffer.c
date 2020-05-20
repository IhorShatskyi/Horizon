#include "bitBuffer.h"
#include "list.h"
#include "storonaDriver.h"
#include "stdlib.h"

#define SIZE_BUFFER (768*8)
#define SINCHRO_LENGHT (3*2)

// Master functions
static void BitAdd(uint8_t);
static uint8_t BitGet(void);

// Slave functions
static void CheckPercent(void);

// Master struct
const struct bitBufferFunctions BitBuffer = {
	{
		BitAdd,
		BitGet
	}
};

static uint8_t bitBuffer[SIZE_BUFFER];
list listBitBuffer = {"Bit Buffer", bitBuffer, SIZE_BUFFER, 0, 0};

// Description master function
static void BitAdd(uint8_t bit){
	List.Write.Ubyte(&listBitBuffer, bit);
	CheckPercent();
}
static uint8_t BitGet(void){
	static uint8_t bit = 0;

	static uint8_t synchro[SINCHRO_LENGHT] = {1,1, 1,0, 0,0};
	//static uint32_t initFlag = 0;
	static uint32_t counter = 0;
	/*
	if(!initFlag){
		initFlag = 1;
		for(uint32_t i = 0; i < SINCHRO_LENGHT; i++)
			synchro[i] = (uint8_t)rand() & 0x1;
	}
	*/
	
	if(List.Count(&listBitBuffer) == 0){		
		bit = synchro[counter % SINCHRO_LENGHT];
		counter++;
	}	else {
		bit = List.Read.Ubyte(&listBitBuffer);
	}
	CheckPercent();
	return bit;
}

// Description slave function
static void CheckPercent(void){
	static int32_t oldPercent = 0;
	
	// Calc percent
	int32_t newPercent = ( 100 * List.Count(&listBitBuffer) ) / SIZE_BUFFER;

	// Check diff
	if(oldPercent - newPercent){
		oldPercent = newPercent;
		StoronaDriver.Percent((uint8_t)newPercent);
	}

}
