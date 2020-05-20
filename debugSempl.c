#include "debugSempl.h"
#include "stm32f446xx.h"
#include "iq.h"

#define SIZE_DEBUG 8192

// list
static uint8_t listSempl[SIZE_DEBUG];
list Sempls = {"Sempls", listSempl, SIZE_DEBUG, 0, 0};

// Master functions
static void Show(complex);

// Master struct
const struct debugSemplFunctions DebugSempl = {
	Show
};

// Description
static void Show(complex iq){
	static uint32_t semplCounter = 0;
	if(semplCounter++ % 64 == 0)
		List.Write.Uint(&Sempls, 0x12345600);
	List.Write.Complex(&Sempls, iq);
}
