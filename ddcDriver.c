#include "ddcDriver.h"

#define MASK 0x12345600
#define SIZE_TRANSFER 2048

static uint8_t ddcDriver[SIZE_TRANSFER];
list listDdcDriver = {"DDC Driver", ddcDriver, SIZE_TRANSFER, 0, 0};

enum command {
	// 0,
	MODE_SET = 1,
	MODE_GET = 2,
	WIDTH_SET = 3,
	WIDTH_GET = 4,
	FREQUENCY_SET = 5,
	FREQUENCY_GET = 6,
	RESET = 7,
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
static void Reset(void);

// Slave functions
static void Command(uint32_t, uint32_t);

// Main functions struct
const struct ddcDriverFunctions DdcDriver = {
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
static void Reset(void){Command(RESET, 0);}

// Description slave functions
static void Command(uint32_t command, uint32_t data) {
	List.Write.Uint(&listDdcDriver, MASK | command);
	List.Write.Uint(&listDdcDriver, data);
}
