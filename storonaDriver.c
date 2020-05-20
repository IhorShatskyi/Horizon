#include "storonaDriver.h"
#include "print.h"
#include "device.h"

#define PACKET_SIZE 40
#define SIZE_TRANSFER 2048
#define INIT_BYTE 128

static uint8_t storonaDriver[SIZE_TRANSFER];
list listStoronaDriver = {"Storona Driver", storonaDriver, SIZE_TRANSFER, 0, 0};

// Master functions
static void Init(void);	
static void Ethernet(uint32_t, uint32_t, uint16_t);
static void Bit(uint8_t);
static void Percent(uint8_t);

// Master struct
const struct storonaDriverFunctions StoronaDriver = {
	Init,
	Ethernet,
	Bit,
	Percent
};

static void Init(void){
	List.Write.Ubyte(&listStoronaDriver, INIT_BYTE);
	Print("Init "); PrintLn(INIT_BYTE);
}
static void Ethernet(uint32_t ip, uint32_t mask, uint16_t port){
	
	port = 80;
	
	// send ip
	uint8_t *p = (uint8_t *)(&ip) + 4;
	List.Write.Ubyte(&listStoronaDriver, *--p);
	List.Write.Ubyte(&listStoronaDriver, *--p);
	List.Write.Ubyte(&listStoronaDriver, *--p);
	List.Write.Ubyte(&listStoronaDriver, *--p);
	
	// send mask
	p = (uint8_t *)(&mask) + 4;
	List.Write.Ubyte(&listStoronaDriver, *--p);
	List.Write.Ubyte(&listStoronaDriver, *--p);
	List.Write.Ubyte(&listStoronaDriver, *--p);
	List.Write.Ubyte(&listStoronaDriver, *--p);

	// send port
	p = (uint8_t *)(&port) + 2;
	List.Write.Ubyte(&listStoronaDriver, *--p);
	List.Write.Ubyte(&listStoronaDriver, *--p);

	// Debug
	p = (uint8_t *)&ip + 4;
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
	
}
static void Bit(uint8_t bit){
	
	static uint8_t bitCounter = 0;
	
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
	
	Converter.Byte <<= 1;
	Converter.Bits.b0 = bit;
	
	bitCounter++;	

	if(bitCounter != 6) return;
	bitCounter = 0;
	Converter.Bits.mask = 0x03;

	List.Write.Ubyte(&listStoronaDriver, Converter.Byte);
	
}
static void Percent(uint8_t percent){
	//Print("Send Percent "); PrintLn(percent);
	List.Write.Ubyte(&listStoronaDriver, percent);
}
