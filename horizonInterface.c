#include "horizonInterface.h"
#include "horizonDriver.h"
#include "print.h"
#include "flash.h"
#include "init.h"
#include "wiznet.h"
#include "define.h"

#define INIT "Horizon"
#define ERROR_ACTIVE_INTERFACE 19
#define ERROR_DEBUG_MODE 31

enum inerfaceType {
	ETH = 0,
	USB = 1
};

enum debugModeType {
	OFF = 0,
	ON = 1
};

// Master functions
void ActiveInterfaceSet(uint32_t);
void ActiveInterfaceGet(void);
void EthernetSettingsSet(uint32_t, uint32_t, uint32_t, uint32_t);
void EthernetSettingsGet(void);
void EthernetSettingsReset(void);
void GetInit(void);
void SetDebugMode(uint32_t);

// Master struct
struct horizonInterfaceFunctions HorizonInterface = {
	
	{
		ETH,
		ON
	},
	
	{
		ActiveInterfaceSet,
		ActiveInterfaceGet
	},
	
	{
		EthernetSettingsSet,
		EthernetSettingsGet,
		EthernetSettingsReset
	},
	
	GetInit,

	SetDebugMode

};

// Description master functions
void ActiveInterfaceSet(uint32_t interface){
	if(interface > 2){
		HorizonDriver.Error(ERROR_ACTIVE_INTERFACE);	
		// Debug
		Print("ERROR: Active Interface "); PrintLn(interface);
	} else {
		HorizonInterface.Parameter.ActiveInterface = interface;
	}
}
void ActiveInterfaceGet(void){
	HorizonDriver.ActiveInterface(HorizonInterface.Parameter.ActiveInterface);
}
void EthernetSettingsSet(uint32_t ip, uint32_t mask, uint32_t port, uint32_t gateWay){
	// Save to flash
	Flash.Unlock(); 
	Flash.EraseSector(7);
	Flash.Write(0x00, ip);
	Flash.Write(0x04, mask);
	Flash.Write(0x08, port);	
	Flash.Write(0x0C, gateWay);
	Flash.Lock();

	// Reinit Wiznet
	Init.W5500();
}
void EthernetSettingsGet(void){
	uint32_t ip = Wiznet.Common.SourceIpAddress.Get();
	uint32_t mask = Wiznet.Common.SubnetMaskAddress.Get();
	uint32_t port = Wiznet.Socket.SourcePort.Get(SOCKET0);
	uint32_t gateWay = Wiznet.Common.GatewayAddress.Get();
	HorizonDriver.EthernetSettings(ip, mask, port, gateWay);
}
void EthernetSettingsReset(void){
	// Save to flash
	Flash.Unlock(); 
	Flash.EraseSector(7);
	Flash.Write(0x00, IP_DEFAULT);
	Flash.Write(0x04, MASK_DEFAULT);
	Flash.Write(0x08, PORT_DEFAULT);	
	Flash.Write(0x0C, GATEWAY_DEFAULT);
	Flash.Lock();

	// Reinit Wiznet
	Init.W5500();
}
void GetInit(void){
	HorizonDriver.Init(INIT);
}
void SetDebugMode(uint32_t mode){
	if(mode > 2){
		HorizonDriver.Error(ERROR_DEBUG_MODE);	
		// Debug
		Print("ERROR: Debug mode "); PrintLn(mode);
	} else {
		HorizonInterface.Parameter.DebugMode = mode;
	}
}
