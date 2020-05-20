#include "ducParser.h"
#include "print.h"
#include "define.h"

#define MASK 0x12345600
#define TIME_DATA_COLLECT 100 // [ms]

enum state {
	// 0,
	// 1,
	// 2,
	// 3,
	// 4,
	// 5,
	// 6,
	// 7,
	// 8,
	// 9,
	MODE = 10,
	// 11,
	WIDTH = 12,
	// 13,
	FREQUENCY = 14,
	// 15,
	// 16,
	PERCENT = 17,
	// 18,
	// 19,
	STATUS_BAR_MODE = 20,
	STATUS_BAR_WIDTH = 21,
	STATUS_BAR_FREQUENCY = 22,
	// 23,
	// 24,
	// 25,
	// 26,
	// 27,
	// 28,
	// 29,
	// 30,
	ERROR = 31,
	MASK_FIND = 32
};

static uint32_t State = MASK_FIND;
static uint32_t ByteCollecter = 0;
static uint32_t ByteCounter = 0;
static uint32_t Timer = 0;	

// Master functions
static void Parse(list *);

// Slave functions
static void MaskFind(void);
static void Mode(void);
static void Width(void);
static void Frequency(void);
static void Percent(void);
static void Error(void);
static void StatusBarMode(void);
static void StatusBarWidth(void);
static void StatusBarFrequency(void);
static void Reserved(void);

// Main functions struct
struct ducParserFunctions DucParser = {
	
	Parse,
	
	{
		0,
		0,
		0,
		0,
		0,
		{
			0,
			0,
			0,
			0
		}
	}
};

// Handlers table
static void (*stateHandler[])(void) = {
	[0] = Reserved,
	[1] = Reserved,
	[2] = Reserved,
	[3] = Reserved,
	[4] = Reserved,
	[5] = Reserved,
	[6] = Reserved,
	[7] = Reserved,
	[8] = Reserved,
	[9] = Reserved,
	[MODE] = Mode,
	[11] = Reserved,
	[WIDTH] = Width,
	[13] = Reserved,
	[FREQUENCY] = Frequency,
	[15] = Reserved,
	[16] = Reserved,
	[PERCENT] = Percent,
	[18] = Reserved,
	[19] = Reserved,
	[STATUS_BAR_MODE] = StatusBarMode,
	[STATUS_BAR_WIDTH] = StatusBarWidth,
	[STATUS_BAR_FREQUENCY] = StatusBarFrequency,
	[23] = Reserved,
	[24] = Reserved,
	[25] = Reserved,
	[26] = Reserved,
	[27] = Reserved,
	[28] = Reserved,
	[29] = Reserved,
	[30] = Reserved,
	[ERROR] = Error,
	[MASK_FIND] = MaskFind
};

// Description master functions
static void Parse(list *list) {
	uint32_t count = List.Count(list);
	while(count--){
		ByteCollecter >>= 8;
		ByteCollecter |= ((uint32_t)List.Read.Ubyte(list) << 24);
		ByteCounter++;
		stateHandler[State]();
	}
	if(State == MASK_FIND) return;
	if(SYSTIMER - Timer < TIME_DATA_COLLECT) return;
	Print("ERROR: DUC timeout collecting command "); PrintLn(State);
	State = MASK_FIND;
}

// Description slave functions
static void MaskFind(void) {
	if((ByteCollecter & 0xFFFFFFE0) != MASK) return;
	State = ByteCollecter & 0x0000001F;
	ByteCounter = 0;
	Timer = SYSTIMER;
}
static void Mode(void){
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	if(DucParser.Event.Mode)
		DucParser.Event.Mode(ByteCollecter);
}
static void Width(void){
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	if(DucParser.Event.Width)
		DucParser.Event.Width(ByteCollecter);
}
static void Frequency(void){
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	if(DucParser.Event.Frequency)
		DucParser.Event.Frequency(ByteCollecter);
}
static void Percent(void){
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	if(DucParser.Event.Percent)
		DucParser.Event.Percent(ByteCollecter);
	if(DucParser.Event.StatusBar.Percent)
		DucParser.Event.StatusBar.Percent(ByteCollecter);
}
static void Error(void){
	if(ByteCounter != 4) return;
	Print("ERROR: DUC returned error "); PrintLn(ByteCollecter);	
	State = MASK_FIND;
	if(DucParser.Event.Error)
		DucParser.Event.Error(ByteCollecter);
}
static void StatusBarMode(void){
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	if(DucParser.Event.StatusBar.Mode)
		DucParser.Event.StatusBar.Mode(ByteCollecter);
}
static void StatusBarWidth(void){
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	if(DucParser.Event.StatusBar.Width)
		DucParser.Event.StatusBar.Width(ByteCollecter);
}
static void StatusBarFrequency(void){
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	if(DucParser.Event.StatusBar.Frequency)
		DucParser.Event.StatusBar.Frequency(ByteCollecter);
}
static void Reserved(void){
	Print("ERROR: DUC returned unused command "); PrintLn(State);
	State = MASK_FIND;
	stateHandler[State]();
}
