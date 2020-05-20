#include "horizonDriver.h"
#include "print.h"

#define MASK 0x12345600
#define PACKET_SIZE 64
#define SIZE_TRANSFER 2048

typedef struct {
	int16_t re;
	int16_t im;
} sempl;

static uint8_t horizonDriver[SIZE_TRANSFER];
list listHorizonDriver = {"Horizon Driver", horizonDriver, SIZE_TRANSFER, 0, 0};

enum command {
	PACKET = 0,
	// 1,
	DDC_MODE = 2,
	// 3,
	DDC_WIDTH = 4,
	// 5,
	DDC_FREQUENCY = 6,
	// 7,
	// 8,
	// 9,
	DUC_MODE = 10,
	// 11,
	DUC_WIDTH = 12,
	// 13,
	DUC_FREQUENCY = 14,
	// 15,
	// 16,
	DUC_PERCENT = 17,
	// 18,
	// 19,
	// 20,
	// 21,
	ETHERNET = 22,
	// 23,
	// 24,
	// 25,
	// 26,
	// 27,
	// 28,
	// 29,
	INIT = 30,
	ERROR = 31
};

// Master functions
static void DdcMode(uint32_t);
static void DdcWidth(uint32_t);
static void DdcFrequency(uint32_t);
static void DdcSempl(complex);
static void DucMode(uint32_t);
static void DucWidth(uint32_t);
static void DucFrequency(uint32_t);
static void DucPercent(uint32_t);
static void Ethernet(uint32_t, uint32_t, uint32_t, uint32_t);
static void Init(char *);
static void Error(uint32_t);

// Slave functions
static void Command(uint32_t, uint32_t);
static sempl Converter(complex);

// Main functions struct
const struct horizonDriverFunctions HorizonDriver = {

	{
		DdcMode,
		DdcWidth,
		DdcFrequency,
		DdcSempl
	},
	
	{
		DucMode,
		DucWidth,
		DucFrequency,
		DucPercent		
	},

	Ethernet,
	Init,
	Error

};

// Description master functions
static void DdcMode(uint32_t mode) {
	Command(DDC_MODE, mode);
	Print("Mode DDC "); PrintLn(mode);
}
static void DdcWidth(uint32_t width) {
	Command(DDC_WIDTH, width);
	Print("Width DDC "); PrintLn(width);
}
static void DdcFrequency(uint32_t frequency) {
	Command(DDC_FREQUENCY, frequency);
	Print("Frequency DDC "); PrintLn(frequency);
}
static void DdcSempl(complex src) {
	static sempl Packet[PACKET_SIZE];
	static uint32_t Counter = 0;
	Packet[Counter++] = Converter(src);
	if(Counter != PACKET_SIZE) return;
	Counter = 0;
	List.Write.Uint(&listHorizonDriver, MASK | PACKET);
	for(uint32_t i = 0; i < PACKET_SIZE; i++){
		List.Write.Short(&listHorizonDriver, Packet[i].re);
		List.Write.Short(&listHorizonDriver, Packet[i].im);
	}
}
static void DucMode(uint32_t mode) {
	Command(DUC_MODE, mode);
	Print("Mode DUC "); PrintLn(mode);
}
static void DucWidth(uint32_t width) {
	Command(DUC_WIDTH, width);
	Print("Width DUC "); PrintLn(width);
}
static void DucFrequency(uint32_t frequency) {
	Command(DUC_FREQUENCY, frequency);
	Print("Frequency DUC "); PrintLn(frequency);
}
static void DucPercent(uint32_t percent) {
	Command(DUC_PERCENT, percent);
}
static void Ethernet(uint32_t ip, uint32_t mask, uint32_t port, uint32_t gateWay) {
	List.Write.Uint(&listHorizonDriver, MASK | ETHERNET);
	List.Write.Uint(&listHorizonDriver, ip);
	List.Write.Uint(&listHorizonDriver, mask);
	List.Write.Uint(&listHorizonDriver, port);
	List.Write.Uint(&listHorizonDriver, gateWay);

	// Debug
	uint8_t *p = (uint8_t *)&ip + 4;
	Print("Ip "); Print(*--p);
	Print(".");	Print(*--p);
	Print(".");	Print(*--p);
	Print(".");	PrintLn(*--p);
	p = (uint8_t *)&mask + 4;
	Print("Mask "); Print(*--p);
	Print("."); Print(*--p);
	Print("."); Print(*--p);
	Print("."); PrintLn(*--p);
	Print("Port "); PrintLn(port);
	p = (uint8_t *)&gateWay + 4;
	Print("GateWay "); Print(*--p);
	Print("."); Print(*--p);
	Print("."); Print(*--p);
	Print("."); PrintLn(*--p);
}
static void Init(char *string) {
	List.Write.Uint(&listHorizonDriver, MASK | INIT);
	List.Write.String(&listHorizonDriver, string);
	Print("Init "); PrintLn(string);
}
static void Error(uint32_t error) {
	Command(ERROR, error);
	Print("ERROR "); PrintLn(error);
}

// Description slave functions
static void Command(uint32_t command, uint32_t data) {
	List.Write.Uint(&listHorizonDriver, MASK | command);
	List.Write.Uint(&listHorizonDriver, data);
}
static sempl Converter(complex src){
	static sempl Sempl;
	Sempl.re = (int16_t)(src.re * 32767.f);
	Sempl.im = (int16_t)(src.im * 32767.f);
	return Sempl;
}
