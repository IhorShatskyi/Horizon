#ifndef DUC_DRIVER_H
#define DUC_DRIVER_H

#include "stm32f446xx.h"
#include "list.h"
#include "iq.h"

extern list listDucDriver;

extern const struct ducDriverFunctions {

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
	
	struct {
		void (*const Percent)(void);		
		void (*const Clear)(void);
		void (*const Write)(complex);
	} Buffer;

	void (*const Reset)(void);

} DucDriver;

#endif
