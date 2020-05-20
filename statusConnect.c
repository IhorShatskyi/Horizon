#include "statusConnect.h"
#include "wiznet.h"
#include "print.h"

// master functions
static enum status Eth(void);
static enum status Usb(void);

// struct functions
const struct statusConnectFunctions StatusConnect = {
	Eth,
	Usb
};

// describe master functions
static enum status Eth(void){
	enum status status = DISCONNECT;
	if(Wiznet.Common.PhyConfiguration.LinkStatus.Get()) status = CONNECT;

	// Debug
	static enum status oldStatus = DISCONNECT;
	const char *statusString[] = {[DISCONNECT] = "dislink", [CONNECT] = "link"};
	if(status != oldStatus){
		oldStatus = status;		
		Print("Ethernet "); PrintLn(statusString[oldStatus]);
	}

	return status;
}
static enum status Usb(void){
	enum status status = DISCONNECT;
	if(!(GPIOA->IDR & GPIO_IDR_ID8)) status = CONNECT;

	// Debug
	static enum status oldStatus = DISCONNECT;
	const char *statusString[] = {[DISCONNECT] = "dislink", [CONNECT] = "link"};
	if(status != oldStatus){
		oldStatus = status;		
		Print("Usb "); PrintLn(statusString[oldStatus]);
	}

	return status;
}
