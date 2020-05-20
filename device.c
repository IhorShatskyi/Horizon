#include "device.h"
#include "flash.h"
#include "init.h"
#include "wiznet.h"
#include "delay.h"
#include "print.h"

// ETHERNET DEFAULT
#define IP_DEFAULT 0xC0A80001 // 192.168.0.1
#define MASK_DEFAULT 0xFFFFFF00 // 255.255.255.0
#define MAC_ADDRESS 0x123478789067

// WIZNET RESET
#define WIZNET_RESET_ON GPIOB->BSRR = GPIO_BSRR_BS_6
#define WIZNET_RESET_OFF GPIOB->BSRR = GPIO_BSRR_BR_6

// INIT
#define INIT "Horizon"

static void DebugSet(uint32_t);
static uint32_t DebugGet(void);
static void EthernetSet(ethernet);
static ethernet EthernetGet(void);
static void EthernetReset(void);
static char *InitString(void);

static uint32_t Debug = 1;

struct deviceFunctions Device = {
	{
		DebugSet,
		DebugGet
	},
	{
		EthernetSet,
		EthernetGet,
		EthernetReset
	},
	InitString,
	{
		0
	}
};

// Describe master functions
static void DebugSet(uint32_t mode){
	if(mode > 2){
		if(Device.Event.ErrorDebug)
			Device.Event.ErrorDebug(mode);
	} else
		Debug = mode;
}
static uint32_t DebugGet(void){
	return Debug;
}
static void EthernetSet(ethernet parameter){

	// Hardware reset
	WIZNET_RESET_OFF; 
	Delay.MilliSecond(1);
	WIZNET_RESET_ON;
	Delay.MilliSecond(1);

	// Software reset
	Wiznet.Common.Mode.Set(0x80);
	Delay.MilliSecond(10);
	
	//Save to flash
	Flash.Unlock(); 
	Flash.EraseSector(7);
	Flash.Write(0x00, parameter.Ip);
	Flash.Write(0x04, parameter.Mask);
	Flash.Lock();

	// Ip address
	Wiznet.Common.SourceIpAddress.Set(parameter.Ip);
	
	// Mask address
	Wiznet.Common.SubnetMaskAddress.Set(parameter.Mask);
	
	// Mac address
	Wiznet.Common.SourceHardwareAddress.Set(MAC_ADDRESS);

	// Socket 0 - Horizon
	Wiznet.Socket.Mode.Set(SOCKET0, MR_TCP);
	Wiznet.Socket.SourcePort.Set(SOCKET0, 81);
	Wiznet.Socket.ReceiveBufferSize.Set(SOCKET0, 2);
	Wiznet.Socket.TransmitBufferSize.Set(SOCKET0, 2);

	// Socket 1 - Storona
	Wiznet.Socket.Mode.Set(SOCKET1, MR_TCP);
	Wiznet.Socket.SourcePort.Set(SOCKET1, 80);
	Wiznet.Socket.ReceiveBufferSize.Set(SOCKET1, 2);
	Wiznet.Socket.TransmitBufferSize.Set(SOCKET1, 2);

	// Socket 7 - Debug Sempl
	Wiznet.Socket.Mode.Set(SOCKET7, MR_TCP);
	Wiznet.Socket.SourcePort.Set(SOCKET7, 87);
	Wiznet.Socket.ReceiveBufferSize.Set(SOCKET7, 2);
	Wiznet.Socket.TransmitBufferSize.Set(SOCKET7, 2);

		// Debug
	uint8_t *p = (uint8_t *)(&parameter.Ip) + 4;
	Print("Ip "); Print(*--p);
	Print(".");	Print(*--p);
	Print(".");	Print(*--p);
	Print(".");	PrintLn(*--p);
	p = (uint8_t *)(&parameter.Mask) + 4;
	Print("Mask "); Print(*--p);
	Print("."); Print(*--p);
	Print("."); Print(*--p);
	Print("."); PrintLn(*--p);
}
static ethernet EthernetGet(void){
	static ethernet parameters;
	parameters.Ip = Wiznet.Common.SourceIpAddress.Get();
	parameters.Mask = Wiznet.Common.SubnetMaskAddress.Get();
	return parameters;
}
static void EthernetReset(void){
	ethernet parameters;
	parameters.Ip = IP_DEFAULT;
	parameters.Mask = MASK_DEFAULT;
	EthernetSet(parameters);
}
static char * InitString(void){
	static char init[] = INIT;
	return init;
}
