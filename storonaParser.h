#ifndef STORONA_PARSER_H
#define STORONA_PARSER_H

#include "stm32f446xx.h"
#include "list.h"

extern struct storonaParserFunction {
	
	void (*const Parse)(list *);
	
	struct {
		void (*Init)(void);
		void (*Mode)(uint8_t);
		void (*Stop)(void);
		void (*SizePacket)(uint8_t);
		struct{
			void (*Set)(uint32_t, uint32_t, uint16_t);
			void (*Get)(void);
		} Ethernet;
		void (*Bit)(uint8_t);
		struct {
			void (*Modulator)(uint8_t);	
			void (*Demodulator)(uint8_t);			
		} Combination;
		struct {
			void (*Duc)(uint32_t);
			void (*Ddc)(uint32_t);
		} Frequency;
		struct {
			void (*Set)(uint8_t);
		} Inversion;
	} Event;

} StoronaParser;

#endif
