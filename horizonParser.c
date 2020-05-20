#include "horizonParser.h"
#include "print.h"
#include "define.h"

#define MASK 0x12345600
#define TIME_DATA_COLLECT 1000 // [ms]
#define PACKET_SIZE 64

enum state {
	PACKET = 0,
	DDC_MODE_SET = 1,
	DDC_MODE_GET = 2,
	DDC_WIDTH_SET = 3,
	DDC_WIDTH_GET = 4,
	DDC_FREQUENCY_SET = 5,
	DDC_FREQUENCY_GET = 6,
	DDC_RESET = 7,
	// 8,
	DUC_MODE_SET = 9,
	DUC_MODE_GET = 10,
	DUC_WIDTH_SET = 11,
	DUC_WIDTH_GET = 12,
	DUC_FREQUENCY_SET = 13,
	DUC_FREQUENCY_GET = 14,
	DUC_RESET = 15,
	DUC_BUFFER_CLEAR = 16,
	DUC_BUFFER_PERCENT = 17,
	// 18,
	// 19,
	// 20,
	ETHERNET_SET = 21,
	ETHERNET_GET = 22,
	// 23,
	// 24,
	// 25,
	// 26,
	// 27,
	// 28,
	ETHERNET_RESET = 29,
	INIT = 30,
	// 31,
	MASK_FIND = 32
};

// Master functions
static void Parse(list *);

// Slave functions
static void DdcModeSet(void);
static void DdcModeGet(void);
static void DdcWidthSet(void);
static void DdcWidthGet(void);
static void DdcFrequencySet(void);
static void DdcFrequencyGet(void);
static void DdcReset(void);
static void DucModeSet(void);
static void DucModeGet(void);
static void DucWidthSet(void);
static void DucWidthGet(void);
static void DucFrequencySet(void);
static void DucFrequencyGet(void);
static void DucReset(void);
static void DucBufferClear(void);
static void DucBufferPercent(void);
static void DucBufferWrite(void);
static void EthernetSet(void);
static void EthernetGet(void);
static void EthernetReset(void);
static void Init(void);
static void MaskFind(void);
static void Reserved(void);
static complex Converter(uint32_t);

// Parser parameters
static uint32_t State = MASK_FIND;
static uint32_t ByteCollecter = 0;
static uint32_t ByteCounter = 0;
static uint32_t Timer = 0;

// Main functions struct
struct horizonParserFunctions HorizonParser = {

	Parse,

	{
		{
			{0,	0},
			{0,	0},
			{0,	0},
			0
		},

		{
			{0,	0},
			{0,	0},
			{0,	0},
			0,
			{0,	0, 0}
		},

		{0,	0, 0},

		0
	}

};

// Handlers table
static void (*stateHandler[])(void) = {
	[PACKET] = DucBufferWrite,
	[DDC_MODE_SET] = DdcModeSet,
	[DDC_MODE_GET] = DdcModeGet,
	[DDC_WIDTH_SET] = DdcWidthSet,
	[DDC_WIDTH_GET] = DdcWidthGet,
	[DDC_FREQUENCY_SET] = DdcFrequencySet,
	[DDC_FREQUENCY_GET] = DdcFrequencyGet,
	[DDC_RESET] = DdcReset,
	[8] = Reserved,
	[DUC_MODE_SET] = DucModeSet,
	[DUC_MODE_GET] = DucModeGet,
	[DUC_WIDTH_SET] = DucWidthSet,
	[DUC_WIDTH_GET] = DucWidthGet,
	[DUC_FREQUENCY_SET] = DucFrequencySet,
	[DUC_FREQUENCY_GET] = DucFrequencyGet,
	[DUC_RESET] = DucReset,
	[DUC_BUFFER_CLEAR] = DucBufferClear,
	[DUC_BUFFER_PERCENT] = DucBufferPercent,
	[18] = Reserved,
	[19] = Reserved,
	[20] = Reserved,
	[ETHERNET_SET] = EthernetSet,
	[ETHERNET_GET] = EthernetGet,
	[23] = Reserved,
	[24] = Reserved,
	[25] = Reserved,
	[26] = Reserved,
	[27] = Reserved,
	[28] = Reserved,
	[ETHERNET_RESET] = EthernetReset,
	[INIT] = Init,
	[31] = Reserved,
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
	Print("ERROR: timeout collecting command "); PrintLn(State);
	State = MASK_FIND;
}

// Description slave functions
static void MaskFind(void) {
	if((ByteCollecter & 0xFFFFFFE0) != MASK) return;
	State = ByteCollecter & 0x0000001F;
	ByteCounter = 0;
	Timer = SYSTIMER;
}
static void DdcModeSet(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	Print("Set mode DDC "); PrintLn(ByteCollecter);	
	if(HorizonParser.Event.Ddc.Mode.Set)
		HorizonParser.Event.Ddc.Mode.Set(ByteCollecter);
}
static void DdcModeGet(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	PrintLn("Get mode DDC");
	if(HorizonParser.Event.Ddc.Mode.Get)
		HorizonParser.Event.Ddc.Mode.Get();
}
static void DdcWidthSet(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	Print("Set width DDC "); PrintLn(ByteCollecter);
	if(HorizonParser.Event.Ddc.Width.Set)
		HorizonParser.Event.Ddc.Width.Set(ByteCollecter);
}
static void DdcWidthGet(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	PrintLn("Get width DDC");
	if(HorizonParser.Event.Ddc.Width.Get)
		HorizonParser.Event.Ddc.Width.Get();
}
static void DdcFrequencySet(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	Print("Set frequency DDC "); PrintLn(ByteCollecter);
	if(HorizonParser.Event.Ddc.Frequency.Set)
		HorizonParser.Event.Ddc.Frequency.Set(ByteCollecter);
}
static void DdcFrequencyGet(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	PrintLn("Get frequency DDC");
	if(HorizonParser.Event.Ddc.Frequency.Get)
		HorizonParser.Event.Ddc.Frequency.Get();
}
static void DdcReset(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	PrintLn("Reset DDC");
	if(HorizonParser.Event.Ddc.Reset)
		HorizonParser.Event.Ddc.Reset();
}
static void DucModeSet(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	Print("Set mode DUC "); PrintLn(ByteCollecter);
	if(HorizonParser.Event.Duc.Mode.Set)
		HorizonParser.Event.Duc.Mode.Set(ByteCollecter);
}
static void DucModeGet(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	PrintLn("Get mode DUC");
	if(HorizonParser.Event.Duc.Mode.Get)
		HorizonParser.Event.Duc.Mode.Get();
}
static void DucWidthSet(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	Print("Set width DUC "); PrintLn(ByteCollecter);
	if(HorizonParser.Event.Duc.Width.Set)
		HorizonParser.Event.Duc.Width.Set(ByteCollecter);
}
static void DucWidthGet(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	PrintLn("Get width DUC");
	if(HorizonParser.Event.Duc.Width.Get)
		HorizonParser.Event.Duc.Width.Get();
}
static void DucFrequencySet(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	Print("Set frequency DUC "); PrintLn(ByteCollecter);
	if(HorizonParser.Event.Duc.Frequency.Set)
		HorizonParser.Event.Duc.Frequency.Set(ByteCollecter);
}
static void DucFrequencyGet(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	PrintLn("Get frequency DUC");
	if(HorizonParser.Event.Duc.Frequency.Get)
		HorizonParser.Event.Duc.Frequency.Get();
}
static void DucReset(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	PrintLn("Reset DUC");
	if(HorizonParser.Event.Duc.Reset)
		HorizonParser.Event.Duc.Reset();
}
static void DucBufferClear(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	PrintLn("Clear buffer DUC");
	if(HorizonParser.Event.Duc.Buffer.Clear)
		HorizonParser.Event.Duc.Buffer.Clear();
}
static void DucBufferPercent(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	PrintLn("Percent buffer DUC");
	if(HorizonParser.Event.Duc.Buffer.Percent)
		HorizonParser.Event.Duc.Buffer.Percent();
}
static void DucBufferWrite(void) {
	if(ByteCounter % 3 != 0) return;
	if(HorizonParser.Event.Duc.Buffer.Write)
		HorizonParser.Event.Duc.Buffer.Write(Converter(ByteCollecter));	
	if(ByteCounter != 3 * PACKET_SIZE) return;
	State = MASK_FIND;
}
static void EthernetSet(void) {
	static uint32_t Ip, Mask, Port, GateWay;
	if(ByteCounter == 4) Ip = ByteCollecter;
	if(ByteCounter == 8) Mask = ByteCollecter;
	if(ByteCounter == 12) Port = ByteCollecter;
	if(ByteCounter == 16) GateWay = ByteCollecter;
	if(ByteCounter != 16) return;
	State = MASK_FIND;
	PrintLn("Ethernet set");
	if(HorizonParser.Event.Ethernet.Set)
		HorizonParser.Event.Ethernet.Set(Ip, Mask, Port, GateWay);
}
static void EthernetGet(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	PrintLn("Ethernet get");
	if(HorizonParser.Event.Ethernet.Get)
		HorizonParser.Event.Ethernet.Get();
}
static void EthernetReset(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	PrintLn("Ethernet reset");
	if(HorizonParser.Event.Ethernet.Reset)
		HorizonParser.Event.Ethernet.Reset();
}
static void Init(void) {
	if(ByteCounter != 4) return;
	State = MASK_FIND;
	PrintLn("Init");
	if(HorizonParser.Event.Init)
		HorizonParser.Event.Init();
}
static void Reserved(void) {
	Print("ERROR: unsupported command "); PrintLn(State);
	State = MASK_FIND;
	stateHandler[State]();
}
static complex Converter(uint32_t src){
	static complex result = {0.f, 0.f};
	static union {
		struct {
			int32_t Unused:8;
			int32_t Re:12;
			int32_t Im:12;	
		} Field;
		uint32_t Collector;
	} Converter;
	Converter.Collector = src;
	result.re = (float)Converter.Field.Re / 2048.f;
	result.im = (float)Converter.Field.Im / 2048.f;
	return result;
}
