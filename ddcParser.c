#include "ddcParser.h"
#include "print.h"
#include "define.h"
#include "debugSempl.h"

#define MASK 0x12345600
#define TIME_DATA_COLLECT 100 // [ms]
#define PACKET_SIZE 64

enum state {
	PACKET = 0,
	// 1,
	MODE = 2,
	// 3,
	WIDTH = 4,
	// 5,
	FREQUENCY = 6,
	// 7,
	// 8,
	// 9,
	// 10,
	// 11,
	// 12,
	// 13,
	// 14,
	// 15,
	// 16,
	// 17,
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
static void Packet(void);
static void Error(void);
static void StatusBarMode(void);
static void StatusBarWidth(void);
static void StatusBarFrequency(void);
static void Reserved(void);
static complex Converter(uint32_t);

// Main functions struct
struct ddcParserFunctions DdcParser = {
	
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
			0
		}
	}

};

// Handlers table
static void (*stateHandler[])(void) = {
	[PACKET] = Packet,
	[1] = Reserved,
	[MODE] = Mode,
	[3] = Reserved,
	[WIDTH] = Width,
	[5] = Reserved,
	[FREQUENCY] = Frequency,
	[7] = Reserved,
	[8] = Reserved,
	[9] = Reserved,
	[10] = Reserved,
	[11] = Reserved,
	[12] = Reserved,
	[13] = Reserved,
	[14] = Reserved,
	[15] = Reserved,
	[16] = Reserved,
	[17] = Reserved,
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
	Print("ERROR: DDC timeout collecting command "); PrintLn(State);
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
	if(DdcParser.Event.Mode)
		DdcParser.Event.Mode(ByteCollecter);
}
static void Width(void){
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	if(DdcParser.Event.Width)
		DdcParser.Event.Width(ByteCollecter);
}
static void Frequency(void){
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	if(DdcParser.Event.Frequency)
		DdcParser.Event.Frequency(ByteCollecter);
}
static void Packet(void){
	if(ByteCounter % 4 != 0) return;
	
	if(DdcParser.Event.Sempl)
		DdcParser.Event.Sempl(Converter(ByteCollecter));

	if(ByteCounter != 4 * PACKET_SIZE) return;
	State = MASK_FIND;
}
static void Error(void){
	if(ByteCounter != 4) return;
	Print("ERROR: DDC returned error "); PrintLn(ByteCollecter);	
	State = MASK_FIND;
	if(DdcParser.Event.Error)
		DdcParser.Event.Error(ByteCollecter);
}
static void StatusBarMode(void){
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	if(DdcParser.Event.StatusBar.Mode)
		DdcParser.Event.StatusBar.Mode(ByteCollecter);
}
static void StatusBarWidth(void){
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	if(DdcParser.Event.StatusBar.Width)
		DdcParser.Event.StatusBar.Width(ByteCollecter);
}
static void StatusBarFrequency(void){
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	if(DdcParser.Event.StatusBar.Frequency)
		DdcParser.Event.StatusBar.Frequency(ByteCollecter);
}
static void Reserved(void){
	Print("ERROR: DDC returned unused command "); PrintLn(State);
	State = MASK_FIND;
	stateHandler[State]();
}
static complex Converter(uint32_t src){
	static complex result = {0.f, 0.f};
	static union {	
		struct {
			int32_t Im:16;
			int32_t Re:16;
		} Sempl;
		uint32_t Collector;
	} Converter;
	Converter.Collector = src;
	result.re = (float)Converter.Sempl.Re / 32768.f;
	result.im = (float)Converter.Sempl.Im / 32768.f;
	return result;
}
