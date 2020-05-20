#ifndef HORIZON_INTERFACE_H
#define HORIZON_INTERFACE_H

#include "stm32f446xx.h"

extern struct horizonInterfaceFunctions {
	
	struct {
		uint32_t ActiveInterface;
		uint32_t DebugMode;
	} Parameter;
	
	struct {
		void (*const Set)(uint32_t);
		void (*const Get)(void);
	} ActiveInterface;
	
	struct {
		void (*const Set)(uint32_t, uint32_t, uint32_t, uint32_t);
		void (*const Get)(void);	
		void (*const Reset)(void);		
	} EthernetSettings;
	
	void (*const GetInit)(void);

	void (*const SetDebugMode)(uint32_t);

} HorizonInterface;

#endif
