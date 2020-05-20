#ifndef SEMPL_SENDER_H
#define SEMPL_SENDER_H

#include "stm32f446xx.h"
#include "iq.h"

extern struct semplSenderFunctions {
	
	void (*const Update)(void);
	
	struct {
		void (*const Set)(uint32_t);
	} Percent;
	
	void (*const Reset)(void);
	
	complex (*SemplSourse)(void);
	
} SemplSender;

#endif



