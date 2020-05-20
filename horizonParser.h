#ifndef HORIZON_PARSER_H
#define HORIZON_PARSER_H

#include "stm32f446xx.h"
#include "list.h"
#include "iq.h"

extern struct horizonParserFunctions {

	void (*const Parse)(list *);

	struct {

		struct {
			
			struct {
				void (*Set)(uint32_t);
				void (*Get)(void);
			} Mode;

			struct {
				void (*Set)(uint32_t);
				void (*Get)(void);
			} Width;

			struct {
				void (*Set)(uint32_t);
				void (*Get)(void);
			} Frequency;

			void (*Reset)(void);

		} Ddc;
		
		struct {
			
			struct {
				void (*Set)(uint32_t);
				void (*Get)(void);
			} Mode;

			struct {
				void (*Set)(uint32_t);
				void (*Get)(void);
			} Width;

			struct {
				void (*Set)(uint32_t);
				void (*Get)(void);
			} Frequency;

			void (*Reset)(void);

			struct {
				void (*Percent)(void);
				void (*Clear)(void);
				void (*Write)(complex);
			} Buffer;

		} Duc;
		
		struct {
			void (*Set)(uint32_t, uint32_t, uint32_t, uint32_t);
			void (*Get)(void);	
			void (*Reset)(void);
		} Ethernet;

		void (*Init)(void);
		
	} Event;
	
} HorizonParser;

#endif
