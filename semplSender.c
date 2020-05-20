#include "semplSender.h"
#include "define.h"
#include "ducDriver.h"
#include "modulatorQFSK.h"
#include "demodulatorQFSK.h"
#include "iq.h"

// Master functions
static void Update(void);
static void PercentSet(uint32_t);
static void Reset(void);

// Master struct
struct semplSenderFunctions SemplSender = {
	Update,
	{
		PercentSet
	},
	Reset,
	0
};

// Parameters
static struct {
	uint32_t PercentDucBuffer;
} Parameter = {
	0
};


// Description master function
static void Reset(void){Parameter.PercentDucBuffer = 0;}

static void PercentSet(uint32_t percent){
	Parameter.PercentDucBuffer = percent;
}


static void Update(void){
	static uint32_t time = 0;
	if(SYSTIMER - time < 1) return;
	time = SYSTIMER;
	if(Parameter.PercentDucBuffer > 10) return;
	
	if(SemplSender.SemplSourse == 0) return;
	
	uint32_t semplCounter = 64;
	while(semplCounter--)
		DucDriver.Buffer.Write(SemplSender.SemplSourse());

}

