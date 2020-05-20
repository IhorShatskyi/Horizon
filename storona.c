#include "storona.h"
#include "list.h"
#include "socket.h"
#include "storonaParser.h"
#include "storonaCore.h"
#include "storonaDriver.h"
#include "define.h"
#include "print.h"
#include "ducParser.h"
#include "ddcParser.h"
#include "ducDriver.h"
#include "ddcDriver.h"
#include "semplSender.h"
#include "modulatorQFSK.h"
#include "demodulatorQFSK.h"

#define SIZE_SOCKET_RX 2048

static uint8_t socket1Rx[SIZE_SOCKET_RX];
static list listSocket1Rx = {"Socket 1 Rx", socket1Rx, SIZE_SOCKET_RX, 0, 0};

static void Init(void);
static void Run(void);
static void Stop(void);

const struct storonaFunctions Storona = {
	Init,
	Run,
	Stop
};

static void Stop(void){
	
	PrintLn("Storona was stoped");
	
	// Reset DUC
	DucDriver.Reset();

	// Reset DDC
	DdcDriver.Reset();

	// Clear DUC Parser 
	DucParser.Event.Error = 0;
	DucParser.Event.Frequency = 0;
	DucParser.Event.Mode = 0;
	DucParser.Event.Percent = 0;
	DucParser.Event.Width = 0;

	// Clear DDC Parser
	DdcParser.Event.Error = 0;
	DdcParser.Event.Frequency = 0;
	DdcParser.Event.Mode = 0;
	DdcParser.Event.Width = 0;
	DdcParser.Event.Sempl = 0;
	
}

static void Init(void){
	
	PrintLn("Storona init");
	
	// Links Parser to Core
	StoronaParser.Event.Bit = StoronaCore.Parser.Bit;
	StoronaParser.Event.Ethernet.Set = StoronaCore.Parser.Ethernet.Set;
	StoronaParser.Event.Ethernet.Get = StoronaCore.Parser.Ethernet.Get;
	StoronaParser.Event.Init = StoronaCore.Parser.Init;
	StoronaParser.Event.Mode = StoronaCore.Parser.Mode;
	StoronaParser.Event.SizePacket = StoronaCore.Parser.SizePacket;
	StoronaParser.Event.Stop = StoronaCore.Parser.Stop;
	StoronaParser.Event.Combination.Modulator = StoronaCore.Parser.Combination.Modulator;
	StoronaParser.Event.Combination.Demodulator = StoronaCore.Parser.Combination.Demodulator;
	StoronaParser.Event.Frequency.Duc = StoronaCore.Parser.Frequency.Duc;
	StoronaParser.Event.Frequency.Ddc = StoronaCore.Parser.Frequency.Ddc;
	StoronaParser.Event.Inversion.Set = StoronaCore.Parser.Inversion.Set;
	
	// Links DUC Parser to Core
	DucParser.Event.Error = StoronaCore.Duc.Error;
	DucParser.Event.Frequency = StoronaCore.Duc.Frequency;
	DucParser.Event.Mode = StoronaCore.Duc.Mode;
	DucParser.Event.Percent = StoronaCore.Duc.Percent;
	DucParser.Event.Width = StoronaCore.Duc.Width;

	// Links DDC Parser to Core
	DdcParser.Event.Error = StoronaCore.Ddc.Error;
	DdcParser.Event.Frequency = StoronaCore.Ddc.Frequency;
	DdcParser.Event.Mode = StoronaCore.Ddc.Mode;
	DdcParser.Event.Width = StoronaCore.Ddc.Width;
	DdcParser.Event.Sempl = StoronaCore.Ddc.Sempl;
	
	// Links Modulator to Sempl Sender
	SemplSender.SemplSourse = ModulatorQFSK.GetSempl;

	// Core
	StoronaCore.Reset();

	// Modulator
	ModulatorQFSK.Reset();
	
	// Dedulator
	DemodulatorQFSK.Reset();
	
	SemplSender.Reset();
	
	// Config DUC
	DucDriver.Mode.Set(1);
	DucDriver.Width.Set(2);
	DucDriver.Frequency.Set(128000);
	
	// Config DDC
	DdcDriver.Mode.Set(1);
	DdcDriver.Width.Set(2);
	DdcDriver.Frequency.Set(128000);

}

static void Run(void){
	
	// Read from socket
	Socket.Read(SOCKET1, &listSocket1Rx);
	
	// Over parser
	StoronaParser.Parse(&listSocket1Rx);
	
	// Send Sempls
	SemplSender.Update();
	
	// Send to socket
	Socket.Write(SOCKET1, &listStoronaDriver);
}

