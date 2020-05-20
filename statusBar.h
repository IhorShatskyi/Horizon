#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include "stm32f446xx.h"

extern const struct statusBarFunctions {
	
	struct{
		void (*const Mode)(uint32_t);
		void (*const Width)(uint32_t);
		void (*const Frequency)(uint32_t);
	} Ddc;
	
	struct{
		void (*const Mode)(uint32_t);
		void (*const Width)(uint32_t);
		void (*const Frequency)(uint32_t);
		void (*const Percent)(uint32_t);
	} Duc;
	
	void (*const Show)(void);

} StatusBar;

#endif
