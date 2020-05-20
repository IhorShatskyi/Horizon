#include "button.h"
#include "stm32f446xx.h"
#include "led.h"
#include "define.h"
#include "flash.h"
#include "init.h"
#include "print.h"
#include "device.h"

#define TIME_PRESS_BUTTON 3000 // [ms]

static void Check(void);

struct buttonFunctions Button = {
	Check
};

static void Check(void){
	static uint32_t flag = 0, time = 0;

	// check press
	if(GPIOC->IDR & GPIO_IDR_ID11) {
		flag = 0;
		return;
	}
	
	// light on
	RED_ON; GREEN_ON; BLUE_ON;
        
	// fix time
	if (!flag ) {  
		flag = 1;
		time = SYSTIMER;  
	}
            
	// check time
	if(SYSTIMER - time < TIME_PRESS_BUTTON) return;
		flag = 0;
	
	// light off
	RED_OFF; GREEN_OFF; BLUE_OFF;
	
	// Debug
	PrintLn("Press button");
	
	Device.Ethernet.Reset();
}

