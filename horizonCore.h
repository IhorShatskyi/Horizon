#ifndef HORIZON_CORE_H
#define HORIZON_CORE_H

#include "stm32f446xx.h"
#include "iq.h"

extern const struct horizonCoreFunctions {

	struct {
		void (*const Mode)(uint32_t);
		void (*const Width)(uint32_t);
		void (*const Frequency)(uint32_t);
		void (*const Sempl)(complex);
		void (*const Error)(uint32_t);
	} Ddc;

	struct {
		void (*const Mode)(uint32_t);
		void (*const Width)(uint32_t);
		void (*const Frequency)(uint32_t);
		void (*const Percent)(uint32_t);
		void (*const Error)(uint32_t);
	} Duc;

	struct {
		
		struct {
			
			struct {
				void (*const Set)(uint32_t);
				void (*const Get)(void);				
			} Mode;
			
			struct {
				void (*const Set)(uint32_t);
				void (*const Get)(void);				
			} Width;
			
			struct {
				void (*const Set)(uint32_t);
				void (*const Get)(void);				
			} Frequency;
			
			void (*const Reset)(void);
			
		} Ddc;

		struct {

			struct {
				void (*const Set)(uint32_t);
				void (*const Get)(void);
			} Mode;

			struct {
				void (*const Set)(uint32_t);
				void (*const Get)(void);
			} Width;

			struct {
				void (*const Set)(uint32_t);
				void (*const Get)(void);
			} Frequency;

			void (*const Reset)(void);

			struct {
				void (*const Percent)(void);
				void (*const Clear)(void);
				void (*const Write)(complex);
			} Buffer;

		} Duc;

		struct {
			void (*const Set)(uint32_t, uint32_t, uint32_t, uint32_t);
			void (*const Get)(void);	
			void (*const Reset)(void);		
		} Ethernet;
		
		void (*const Init)(void);
		
	} Parser;

} HorizonCore;

#endif
