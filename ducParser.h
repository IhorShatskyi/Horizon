#ifndef DUC_PARSER_H
#define DUC_PARSER_H

#include "stm32f446xx.h"
#include "list.h"

extern struct ducParserFunctions {

	void (*const Parse)(list *);

	struct {
		
		void (*Mode)(uint32_t);
		void (*Width)(uint32_t);
		void (*Frequency)(uint32_t);
		void (*Percent)(uint32_t);
		void (*Error)(uint32_t);
		
		struct {
			void (*Mode)(uint32_t);
			void (*Width)(uint32_t);
			void (*Frequency)(uint32_t);
			void (*Percent)(uint32_t);
		} StatusBar;
		
	} Event;
	
} DucParser;

#endif
