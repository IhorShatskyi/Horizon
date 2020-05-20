#include "horizon.h"
#include "list.h"
#include "socket.h"
#include "horizonParser.h"
#include "horizonDriver.h"
#include "horizonCore.h"
#include "ducParser.h"
#include "ddcParser.h"
#include "ducDriver.h"
#include "ddcDriver.h"
#include "print.h"

#define SIZE_SOCKET_RX 2048

static uint8_t socket0Rx[SIZE_SOCKET_RX];
static list listSocket0Rx = {"Socket 0 Rx", socket0Rx, SIZE_SOCKET_RX, 0, 0};

static void Init(void);
static void Run(void);
static void Stop(void);

const struct horizonFunctions Horizon = {
	Init,
	Run,
	Stop
};

static void Stop(void){
	
	PrintLn("Horizon was stoped");

	// Reset DUC
	DucDriver.Reset();

	// Reset DDC
	DdcDriver.Reset();

	// Clear DUC Parser to Core
	DucParser.Event.Mode = 0;
	DucParser.Event.Width = 0;
	DucParser.Event.Frequency = 0;
	DucParser.Event.Percent = 0;
	DucParser.Event.Error = 0;

	// Clear DDC Parser to Core
	DdcParser.Event.Mode = 0;
	DdcParser.Event.Width = 0;
	DdcParser.Event.Frequency = 0;
	DdcParser.Event.Sempl = 0;
	DdcParser.Event.Error = 0;
	
}


static void Init(void){
	
	PrintLn("Horizon init");
	
	// Links Parser to Core
	HorizonParser.Event.Duc.Mode.Set = HorizonCore.Parser.Duc.Mode.Set;
	HorizonParser.Event.Duc.Mode.Get = HorizonCore.Parser.Duc.Mode.Get;
	HorizonParser.Event.Duc.Width.Set = HorizonCore.Parser.Duc.Width.Set;
	HorizonParser.Event.Duc.Width.Get = HorizonCore.Parser.Duc.Width.Get;
	HorizonParser.Event.Duc.Frequency.Set = HorizonCore.Parser.Duc.Frequency.Set;
	HorizonParser.Event.Duc.Frequency.Get = HorizonCore.Parser.Duc.Frequency.Get;
	HorizonParser.Event.Duc.Reset = HorizonCore.Parser.Duc.Reset;
	HorizonParser.Event.Duc.Buffer.Clear = HorizonCore.Parser.Duc.Buffer.Clear;
	HorizonParser.Event.Duc.Buffer.Percent = HorizonCore.Parser.Duc.Buffer.Percent;
	HorizonParser.Event.Duc.Buffer.Write = HorizonCore.Parser.Duc.Buffer.Write;
	HorizonParser.Event.Ddc.Mode.Set = HorizonCore.Parser.Ddc.Mode.Set;
	HorizonParser.Event.Ddc.Mode.Get = HorizonCore.Parser.Ddc.Mode.Get;
	HorizonParser.Event.Ddc.Width.Set = HorizonCore.Parser.Ddc.Width.Set;
	HorizonParser.Event.Ddc.Width.Get = HorizonCore.Parser.Ddc.Width.Get;
	HorizonParser.Event.Ddc.Frequency.Set = HorizonCore.Parser.Ddc.Frequency.Set;
	HorizonParser.Event.Ddc.Frequency.Get = HorizonCore.Parser.Ddc.Frequency.Get;
	HorizonParser.Event.Ddc.Reset = HorizonCore.Parser.Ddc.Reset;
	HorizonParser.Event.Ethernet.Set = HorizonCore.Parser.Ethernet.Set;
	HorizonParser.Event.Ethernet.Get = HorizonCore.Parser.Ethernet.Get;
	HorizonParser.Event.Ethernet.Reset = HorizonCore.Parser.Ethernet.Reset;
	HorizonParser.Event.Init = HorizonCore.Parser.Init;
	
	// Links DUC Parser to Core
	DucParser.Event.Mode = HorizonCore.Duc.Mode;
	DucParser.Event.Width = HorizonCore.Duc.Width;
	DucParser.Event.Frequency = HorizonCore.Duc.Frequency;
	DucParser.Event.Percent = HorizonCore.Duc.Percent;
	DucParser.Event.Error = HorizonCore.Duc.Error;

	// Links DDC Parser to Core
	DdcParser.Event.Mode = HorizonCore.Ddc.Mode;
	DdcParser.Event.Width = HorizonCore.Ddc.Width;
	DdcParser.Event.Frequency = HorizonCore.Ddc.Frequency;
	DdcParser.Event.Sempl = HorizonCore.Ddc.Sempl;
	DdcParser.Event.Error = HorizonCore.Ddc.Error;
}

static void Run(void){
	Socket.Read(SOCKET0, &listSocket0Rx);
	HorizonParser.Parse(&listSocket0Rx);
	Socket.Write(SOCKET0, &listHorizonDriver);
}

