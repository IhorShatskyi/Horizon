#ifndef STORONA_CORE_H
#define STORONA_CORE_H

#include "stm32f446xx.h"
#include "iq.h"

extern const struct storonaCoreFunctions {

	struct {
		
		void (*const Init)(void);
		void (*const Mode)(uint8_t);
		void (*const Stop)(void);
		void (*const SizePacket)(uint8_t);
		struct{
			void (*const Set)(uint32_t, uint32_t, uint16_t);
			void (*const Get)(void);
		} Ethernet;
		void (*const Bit)(uint8_t);
		struct {
			void (*const Modulator)(uint8_t);
			void (*const Demodulator)(uint8_t);
		} Combination;
		struct {
			void (*const Duc)(uint32_t);
			void (*const Ddc)(uint32_t);
		} Frequency;
		struct {
			void (*const Set)(uint8_t);
		} Inversion;
		
	} Parser;

	struct {
		void (*const Mode)(uint32_t);
		void (*const Width)(uint32_t);
		void (*const Frequency)(uint32_t);
		void (*const Percent)(uint32_t);
		void (*const Error)(uint32_t);
	} Duc;	

	struct {
		void (*const Mode)(uint32_t);
		void (*const Width)(uint32_t);
		void (*const Frequency)(uint32_t);
		void (*const Sempl)(complex);
		void (*const Error)(uint32_t);
	} Ddc;
	
	void (*const Reset)(void);
	
} StoronaCore;

#endif

