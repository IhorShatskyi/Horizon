#include "led.h"
#include "stm32f446xx.h"
#include "define.h"
#include "statusConnect.h"
#include "wiznet.h"

#define TIME_CHECK_CONNECT 90 // [ms]
#define TIME_CHANGE_BRIGHT 50 // [ms]

enum color {R, G, B};

static void ledUpdate(void);
static void blink(enum color);

const struct ledFunctions Led = {
	ledUpdate
};

static void ledUpdate(void){
	
	static enum color colorTable[2][2] = {
		[0][0] = R, [0][1] = B,
		[1][0] = B, [1][1] = G
	};

	static enum color currentColor = R;
	blink(currentColor);
	
	static uint32_t time = 0;	
	if(SYSTIMER - time < TIME_CHECK_CONNECT) return;
	time = SYSTIMER;
	currentColor = colorTable[StatusConnect.Eth()][StatusConnect.Usb()];
}

static void blink(enum color colorLed){
	static uint32_t bright[32] = {1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	static uint32_t index = 0;	

	if((SYSTIMER & 0xF) < bright[index & 0x1F]) {
				 if (colorLed == R) RED_ON;
		else if (colorLed == G) GREEN_ON;
		else if (colorLed == B) BLUE_ON;
	}
	else {RED_OFF; GREEN_OFF; BLUE_OFF;}
	
	static uint32_t time = 0;
	if(SYSTIMER - time < TIME_CHANGE_BRIGHT) return;
	time = SYSTIMER;
	index++;
}
