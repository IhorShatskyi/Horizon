#ifndef DDC_PARSER_H
#define DDC_PARSER_H

#include "stm32f446xx.h"
#include "iq.h"
#include "list.h"

extern struct ddcParserFunctions {

	void (*const Parse)(list *);
	
	struct {
		
		void (*Mode)(uint32_t);
		void (*Width)(uint32_t);
		void (*Frequency)(uint32_t);
		void (*Sempl)(complex);
		void (*Error)(uint32_t);
	
		struct {
			void (*Mode)(uint32_t);
			void (*Width)(uint32_t);
			void (*Frequency)(uint32_t);
		} StatusBar;
		
	} Event;	

} DdcParser;

#endif
