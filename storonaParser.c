#include "storonaParser.h"
#include "define.h"
#include "print.h"

#define TIME_DATA_COLLECT 1000 // [ms]

// Commands of transfer protocol
enum commands {
	INIT = 0,
	MODE = 1,
	STOP = 2,
	SIZE_PACKET = 3,	
	ETHERNET_SET = 4,
	ETHERNET_GET = 5,
	COMBINATION_MOD = 6,
	FREQUENCY_DUC = 7,
	FREQUENCY_DDC = 8,
	COMBINATION_DEM = 9,
	INVERSION = 10,
	// 11,
	// 12,
	// 13,
	// 14,
	// 15,
	// 16,
	// 17,
	// 18,
	// 19,
	// 20,
	// 21,
	// 22,
	// 23,
	// 24,
	// 25,
	// 26,
	// 27,
	// 28,
	// 29,
	// 30,
	// 31,
	// 32,
	// 33,
	// 34,
	// 35,
	// 36,
	// 37,
	// 38,
	// 39,
	// 40,
	// 41,
	// 42,
	// 43,
	// 44,
	// 45,
	// 46,
	// 47,
	// 48,
	// 49,
	// 50,
	// 51,
	// 52,
	// 53,
	// 54,
	// 55,
	// 56,
	// 57,
	// 58,
	// 59,
	// 60,
	// 61,
	// 62,
	// 63,
	DATA = 64,
	DEFAULT = 65
};

// Master functions
static void Parse(list *);

// Slave functions
static void Default(void);
static void Init(void);
static void Mode(void);
static void Stop(void);
static void SizePacket(void);
static void EthernetSet(void);
static void EthernetGet(void);
static void Bit(void);
static void CombinationModulator(void);
static void CombinationDemodulator(void);
static void FrequencyDuc(void);
static void FrequencyDdc(void);
static void InversionSet(void);
static void Reserved(void);
static uint32_t Revers(uint32_t);

// Master struct
struct storonaParserFunction StoronaParser = {
	Parse,
	{
		0,
		0,
		0,
		0,
		{0,	0},
		0,
		{0, 0},
		{0, 0},
		{0}
	}
};

// Parser parameters
static uint32_t State = DEFAULT;
static uint32_t ByteCollecter = 0;
static uint32_t ByteCounter = 0;
static uint32_t Timer = 0;

// Handlers table
static void (*stateHandler[])(void) = {
	[INIT] = Init,
	[MODE] = Mode,
	[STOP] = Stop,
	[SIZE_PACKET] = SizePacket,
	[ETHERNET_SET] = EthernetSet,
	[ETHERNET_GET] = EthernetGet,
	[COMBINATION_MOD] = CombinationModulator,
	[FREQUENCY_DUC] = FrequencyDuc,
	[FREQUENCY_DDC] = FrequencyDdc,
	[COMBINATION_DEM] = CombinationDemodulator,
	[INVERSION] = InversionSet,
	[11] = Reserved,
	[12] = Reserved,
	[13] = Reserved,
	[14] = Reserved,
	[15] = Reserved,
	[16] = Reserved,
	[17] = Reserved,
	[18] = Reserved,
	[19] = Reserved,
	[20] = Reserved,
	[21] = Reserved,
	[22] = Reserved,
	[23] = Reserved,
	[24] = Reserved,
	[25] = Reserved,
	[26] = Reserved,
	[27] = Reserved,
	[28] = Reserved,
	[29] = Reserved,
	[30] = Reserved,
	[31] = Reserved,
	[32] = Reserved,
	[33] = Reserved,
	[34] = Reserved,
	[35] = Reserved,
	[36] = Reserved,
	[37] = Reserved,
	[38] = Reserved,
	[39] = Reserved,
	[40] = Reserved,
	[41] = Reserved,
	[42] = Reserved,
	[43] = Reserved,
	[44] = Reserved,
	[45] = Reserved,
	[46] = Reserved,
	[47] = Reserved,
	[48] = Reserved,
	[49] = Reserved,
	[50] = Reserved,
	[51] = Reserved,
	[52] = Reserved,
	[53] = Reserved,
	[54] = Reserved,
	[55] = Reserved,
	[56] = Reserved,
	[57] = Reserved,
	[58] = Reserved,
	[59] = Reserved,
	[60] = Reserved,
	[61] = Reserved,
	[62] = Reserved,
	[63] = Reserved,
	[DATA] = Bit,
	[DEFAULT] = Default
};

// Description master functions
static void Parse(list *list){
	uint32_t count = List.Count(list);
	
	while(count--){
		ByteCollecter >>= 8;
		ByteCollecter |= ((uint32_t)List.Read.Ubyte(list) << 24);
		ByteCounter++;
		stateHandler[State]();
	}

	if(State == DEFAULT) return;
	if(SYSTIMER - Timer < TIME_DATA_COLLECT) return;
	PrintLn("ERROR: timeout collecting command");
	State = DEFAULT;
}

// Description slave functions
static void Default(void){
	if((ByteCollecter >> 24) >= 192){
		State = DATA;
		stateHandler[State]();		
	}
	if((ByteCollecter >> 24) < 64){
		State = ByteCollecter >> 24;
		ByteCounter = 0;
		Timer = SYSTIMER;
		stateHandler[State]();
	}
}
static void Init(void){
	State = DEFAULT;
	PrintLn("Init");
	if(StoronaParser.Event.Init)
		StoronaParser.Event.Init();
}
static void Mode(void){
	if(ByteCounter != 1) return;
	State = DEFAULT;
	Print("Mode "); PrintLn(ByteCollecter >> 24);
	if(StoronaParser.Event.Mode)
		StoronaParser.Event.Mode(ByteCollecter >> 24);
}
static void Stop(void){
	State = DEFAULT;
	PrintLn("Stop");
	if(StoronaParser.Event.Stop)
		StoronaParser.Event.Stop();
}
static void SizePacket(void){
	if(ByteCounter != 1) return;
	State = DEFAULT;
	Print("Size packet "); PrintLn(ByteCollecter >> 24);
	if(StoronaParser.Event.SizePacket)
		StoronaParser.Event.SizePacket(ByteCollecter >> 24);
}
static void EthernetSet(void){
	static uint32_t Ip, Mask;
	static uint16_t Port;
	if(ByteCounter == 4) Ip = ByteCollecter;
	if(ByteCounter == 8) Mask = ByteCollecter;
	if(ByteCounter == 10) Port = (uint16_t)(ByteCollecter >> 16);
	if(ByteCounter != 10) return;
	State = DEFAULT;
	PrintLn("Ethernet set");
	
	Ip = Revers(Ip);
	Mask = Revers(Mask);	


	if(StoronaParser.Event.Ethernet.Set)
		StoronaParser.Event.Ethernet.Set(Ip, Mask, Port);
}
static void EthernetGet(void){
	State = DEFAULT;
	PrintLn("Ethernet get");
	if(StoronaParser.Event.Ethernet.Get)
		StoronaParser.Event.Ethernet.Get();
}
static void Bit(void){
	
	static union {
		uint8_t Byte;
		struct {
			uint8_t b0:1;
			uint8_t b1:1;
			uint8_t b2:1;
			uint8_t b3:1;
			uint8_t b4:1;
			uint8_t b5:1;
			uint8_t mask:2;
		} Bits;
	} Converter;
	
	State = DEFAULT;	
	
	Converter.Byte = (uint8_t)(ByteCollecter >> 24);
	
	// Debug
	//Print("Data: ");
	//Print(Converter.Bits.b5);
	//Print(Converter.Bits.b4);
	//Print(Converter.Bits.b3);
	//Print(Converter.Bits.b2);
	//Print(Converter.Bits.b1);
	//Print(Converter.Bits.b0);
	//PrintLn("");
	
	
	
	if(StoronaParser.Event.Bit) {
		StoronaParser.Event.Bit(Converter.Bits.b5);
		StoronaParser.Event.Bit(Converter.Bits.b4);
		StoronaParser.Event.Bit(Converter.Bits.b3);
		StoronaParser.Event.Bit(Converter.Bits.b2);
		StoronaParser.Event.Bit(Converter.Bits.b1);
		StoronaParser.Event.Bit(Converter.Bits.b0);		
	}	
	
	
}
static void CombinationModulator(void){
	if(ByteCounter != 1) return;
	State = DEFAULT;
	
	static union {
		uint8_t Byte;
		struct {
			uint8_t F4:2;
			uint8_t F3:2;
			uint8_t F2:2;
			uint8_t F1:2;
		} Position;
	} Converter;
	
	Converter.Byte = (uint8_t)(ByteCollecter >> 24);
	
	Print("Combination Modulator:");
	Print(" F1 = "); Print(Converter.Position.F1);
	Print(" F2 = "); Print(Converter.Position.F2);
	Print(" F3 = "); Print(Converter.Position.F3);
	Print(" F4 = "); Print(Converter.Position.F4);
	PrintLn("");
	
	if(StoronaParser.Event.Combination.Modulator)
		StoronaParser.Event.Combination.Modulator(Converter.Byte);
}
static void CombinationDemodulator(void){
	if(ByteCounter != 1) return;
	State = DEFAULT;
	
	static union {
		uint8_t Byte;
		struct {
			uint8_t F4:2;
			uint8_t F3:2;
			uint8_t F2:2;
			uint8_t F1:2;
		} Position;
	} Converter;
	
	Converter.Byte = (uint8_t)(ByteCollecter >> 24);
	
	Print("Combination Demodulator:");
	Print(" F1 = "); Print(Converter.Position.F1);
	Print(" F2 = "); Print(Converter.Position.F2);
	Print(" F3 = "); Print(Converter.Position.F3);
	Print(" F4 = "); Print(Converter.Position.F4);
	PrintLn("");
	
	if(StoronaParser.Event.Combination.Demodulator)
		StoronaParser.Event.Combination.Demodulator(Converter.Byte);
}
static void FrequencyDuc(void){
	if(ByteCounter != 4) return;
	State = DEFAULT;
	uint32_t frequency = Revers(ByteCollecter);
	Print("Frequency DUC "); PrintLn(frequency);
	if(StoronaParser.Event.Frequency.Duc)
		StoronaParser.Event.Frequency.Duc(frequency);
}
static void FrequencyDdc(void){
	if(ByteCounter != 4) return;
	State = DEFAULT;
	uint32_t frequency = Revers(ByteCollecter);
	Print("Frequency DDC "); PrintLn(frequency);
	if(StoronaParser.Event.Frequency.Ddc)
		StoronaParser.Event.Frequency.Ddc(frequency);
}

static void InversionSet(void){
	if(ByteCounter != 1) return;
	State = DEFAULT;
	
	uint8_t inv = (uint8_t)(ByteCollecter >> 24);
	
	if(inv <= 1){
		Print("Inversion is "); PrintLn(inv);
		if(StoronaParser.Event.Inversion.Set)
			StoronaParser.Event.Inversion.Set(inv);		
	} else {
		PrintLn("Error: Inversion is not correct");
	}
	

}

static void Reserved(void){
	Print("ERROR: unsupported command "); PrintLn(ByteCollecter >> 24);
	State = DEFAULT;
}
static uint32_t Revers(uint32_t src){
	static union {
		
		uint32_t Total;
		
		struct {
			uint32_t b0:8;
			uint32_t b1:8;
			uint32_t b2:8;
			uint32_t b3:8;			
		} Byte;
		
	} Reserver;
	
	
	Reserver.Total = src;
	
	uint8_t b0 = Reserver.Byte.b0;
	uint8_t b1 = Reserver.Byte.b1;	
	uint8_t b2 = Reserver.Byte.b2;	
	uint8_t b3 = Reserver.Byte.b3;

	
	Reserver.Byte.b0 = b3;
	Reserver.Byte.b1 = b2;
	Reserver.Byte.b2 = b1;
	Reserver.Byte.b3 = b0;

	return Reserver.Total;
	
}


