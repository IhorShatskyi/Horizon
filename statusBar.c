#include "statusBar.h"
#include "ducDriver.h"
#include "ddcDriver.h"
#include "define.h"
#include "print.h"
#include "wiznet.h"
#include "statusConnect.h"

#define TIME_UPDATE 90 // [ms]

// Master functions
static void DdcMode(uint32_t);
static void DdcWidth(uint32_t);
static void DdcFrequency(uint32_t);
static void DucMode(uint32_t);
static void DucWidth(uint32_t);
static void DucFrequency(uint32_t);
static void DucPercent(uint32_t);
static void Show(void);

static uint32_t ddcMode = 0, ddcWidth = 0, ddcFrequency = 0;
static uint32_t ducMode = 0, ducWidth = 0, ducFrequency = 0, ducPercent = 0;

const struct statusBarFunctions StatusBar = {
	{
		DdcMode,
		DdcWidth,
		DdcFrequency
	},
	{
		DucMode,
		DucWidth,
		DucFrequency,
		DucPercent
	},
	Show
};

// Description master functions
static void DdcMode(uint32_t mode){ddcMode = mode;}
static void DdcWidth(uint32_t width){ddcWidth = width;}
static void DdcFrequency(uint32_t frequency){ddcFrequency = frequency;}
static void DucMode(uint32_t mode){ducMode = mode;}
static void DucWidth(uint32_t width){ducWidth = width;}
static void DucFrequency(uint32_t frequency){ducFrequency = frequency;}
static void DucPercent(uint32_t percent){ducPercent = percent;}
static void Show(void){
	static uint32_t time = 0;
	if(SYSTIMER - time < TIME_UPDATE) return;
	time = SYSTIMER;

	// status strings
	const char *link[] = {"dlnk", "link"};
	const char *speed[] = {"10M", "100M"};
	const char *mode[] = {"Off", "On", "Gen"};
	const char *width[] = {"48k", "24k", "12k", "6k", "3k"};
	
	// Ethernet
	static uint32_t ipAddress = 0;
	ipAddress = Wiznet.Common.SourceIpAddress.Get();
	uint8_t *p = (uint8_t *)&ipAddress + 4;
	Print("ETH(");
	Print(link[StatusConnect.Eth()]); Print(" ");
	Print(speed[Wiznet.Common.PhyConfiguration.SpeedStatus.Get()]); Print(" ");			
	Print(*--p); Print("."); Print(*--p); Print("."); Print(*--p); Print("."); Print(*--p); Print(") ");

	// USB
	//Print("USB(");
	//Print(link[StatusConnect.Usb()]); Print(") ");

	// DDC
	Print("D("); Print(mode[ddcMode]); Print(" ");
	Print(width[ddcWidth]); Print(" ");
	Print(ddcFrequency); Print("Hz) ");
	DdcDriver.StatusBar.Mode();
	DdcDriver.StatusBar.Width();
	DdcDriver.StatusBar.Frequency();
	
	// DUC
	Print("M("); Print(mode[ducMode]); Print(" ");
	Print(width[ducWidth]); Print(" ");
	Print(ducFrequency);	Print("Hz ");
	Print(ducPercent);	Print("%)");
	DucDriver.StatusBar.Mode();
	DucDriver.StatusBar.Width();
	DucDriver.StatusBar.Frequency();

	// Timer
	PrintTimer(time);

	// Finish string
	PrintStringFinish();
	Print("\r");
}

