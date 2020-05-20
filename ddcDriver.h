#ifndef DDC_DRIVER_H
#define DDC_DRIVER_H

#include "stm32f446xx.h"
#include "list.h"

extern list listDdcDriver;

extern const struct ddcDriverFunctions {
	
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

	struct {
		void (*const Mode)(void);
		void (*const Width)(void);
		void (*const Frequency)(void);
	} StatusBar;

	void (*const Reset)(void);

} DdcDriver;

#endif
