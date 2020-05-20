#include "ducDriver.h"
#include "debugSempl.h"

#define MASK 0x12345600
#define PACKET_SIZE 64
#define SIZE_TRANSFER 2048

static uint8_t ducDriver[SIZE_TRANSFER];
list listDucDriver = {"DUC Driver", ducDriver, SIZE_TRANSFER, 0, 0};

typedef struct {
	uint32_t low:8;
	uint32_t medium:8;
	uint32_t high:8;
	uint32_t unused:8;
} sempl;

enum command {
	BUFFER_WRITE = 0,
	// 1,
	// 2,
	// 3,
	// 4,
	// 5,
	// 6,
	// 7,
	// 8,
	MODE_SET = 9,
	MODE_GET = 10,
	WIDTH_SET = 11,
	WIDTH_GET = 12,
	FREQUENCY_SET = 13,
	FREQUENCY_GET = 14,
	RESET = 15,
	BUFFER_CLEAR = 16,
	BUFFER_PERCENT = 17,
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
	// 31
};

// Master functions
static void ModeSet(uint32_t);
static void ModeGet(void);
static void WidthSet(uint32_t);
static void WidthGet(void);
static void FrequencySet(uint32_t);
static void FrequencyGet(void);
static void StatusBarMode(void);
static void StatusBarWidth(void);
static void StatusBarFrequency(void);
static void BufferPercent(void);
static void BufferClear(void);
static void BufferWrite(complex);
static void Reset(void);

// Slave functions
static void Command(uint32_t, uint32_t);
static sempl Converter(complex);

// Main functions struct
const struct ducDriverFunctions DucDriver = {
	{
		ModeSet,
		ModeGet
	},
	{
		WidthSet,
		WidthGet
	},
	{
		FrequencySet,
		FrequencyGet
	},
	{
		StatusBarMode,
		StatusBarWidth,
		StatusBarFrequency
	},
	{
		BufferPercent,
		BufferClear,
		BufferWrite
	},
	Reset
};

// Description master functions
static void ModeSet(uint32_t mode){Command(MODE_SET, mode);}
static void ModeGet(void){Command(MODE_GET, 0);}
static void WidthSet(uint32_t width){Command(WIDTH_SET, width);}
static void WidthGet(void){Command(WIDTH_GET, 0);}
static void FrequencySet(uint32_t frequency){Command(FREQUENCY_SET, frequency);}
static void FrequencyGet(void){Command(FREQUENCY_GET, 0);}
static void StatusBarMode(void){Command(STATUS_BAR_MODE, 0);}
static void StatusBarWidth(void){Command(STATUS_BAR_WIDTH, 0);}
static void StatusBarFrequency(void){Command(STATUS_BAR_FREQUENCY, 0);}
static void BufferPercent(void){Command(BUFFER_PERCENT, 0);}
static void BufferClear(void){Command(BUFFER_CLEAR, 0);}
static void BufferWrite(complex src){
	static sempl Packet[PACKET_SIZE];
	static uint32_t Counter = 0;
	
	// Test
	//DebugSempl.Show(src);
	
	
	Packet[Counter++] = Converter(src);
	if(Counter != PACKET_SIZE) return;
	Counter = 0;
	List.Write.Uint(&listDucDriver, MASK | BUFFER_WRITE);
	for(uint32_t i = 0; i < PACKET_SIZE; i++){
		List.Write.Ubyte(&listDucDriver, Packet[i].low);	
		List.Write.Ubyte(&listDucDriver, Packet[i].medium);
		List.Write.Ubyte(&listDucDriver, Packet[i].high);
	}
}
static void Reset(void){Command(RESET, 0);}

// Description slave functions
static void Command(uint32_t command, uint32_t data) {
	List.Write.Uint(&listDucDriver, MASK | command);
	List.Write.Uint(&listDucDriver, data);
}
static sempl Converter(complex src){
	static union {
		struct {
			int32_t Re:12;
			int32_t Im:12;
			int32_t Unused:8;			
		} Field;
		sempl Sempl;
	} Converter;
	Converter.Field.Re = (int32_t)(src.re * 2047.f);
	Converter.Field.Im = (int32_t)(src.im * 2047.f);
	return Converter.Sempl;
}
