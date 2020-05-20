#ifndef DEVICE_H
#define DEVICE_H

#include "stm32f446xx.h"

typedef struct {
	uint32_t Ip, Mask;
} ethernet;

extern struct deviceFunctions {
	
	struct {
		void (*const Set)(uint32_t);
		uint32_t (*const Get)(void);
	} Debug;	
	
	struct {
		void (*const Set)(ethernet);
		ethernet (*const Get)(void);	
		void (*const Reset)(void);		
	} Ethernet;

	char *(*const Init)(void);
	
	struct {	
		void (*ErrorDebug)(uint32_t);
	} Event;

} Device;

#endif
