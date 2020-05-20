#include "storonaCore.h"
#include "storonaDriver.h"
#include "device.h"
#include "semplSender.h"
#include "bitBuffer.h"
#include "print.h"
#include "modulatorQFSK.h"
#include "demodulatorQFSK.h"
#include "ducDriver.h"
#include "ddcDriver.h"
#include "storona.h"


// Master functions
static void ParserInit(void);
static void ParserMode(uint8_t);
static void ParserStop(void);
static void ParserSizePacket(uint8_t);
static void ParserEthernetSet(uint32_t, uint32_t, uint16_t);
static void ParserEthernetGet(void);
static void ParserBit(uint8_t);
static void ParserCombinationModulator(uint8_t);
static void ParserCombinationDemodulator(uint8_t);
static void ParserFrequencyDuc(uint32_t);
static void ParserFrequencyDdc(uint32_t);
static void ParserInversionSet(uint8_t);
static void DucMode(uint32_t);
static void DucWidth(uint32_t);
static void DucFrequency(uint32_t);
static void DucPercent(uint32_t);
static void DucError(uint32_t);
static void DdcMode(uint32_t);
static void DdcWidth(uint32_t);
static void DdcFrequency(uint32_t);
static void DdcSempl(complex);
static void DdcError(uint32_t);
static void Reset(void);

// Master struct
const struct storonaCoreFunctions StoronaCore = {
	
	{
		ParserInit,
		ParserMode,
		ParserStop,
		ParserSizePacket,
		{
			ParserEthernetSet,
			ParserEthernetGet
		},
		ParserBit,
		{
			ParserCombinationModulator,
			ParserCombinationDemodulator
		},
		{
			ParserFrequencyDuc,
			ParserFrequencyDdc
		},
		{
			ParserInversionSet
		}
		
	},
	
	{
		DucMode,
		DucWidth,
		DucFrequency,
		DucPercent,
		DucError
	},
	
	{
		DdcMode,
		DdcWidth,
		DdcFrequency,
		DdcSempl,
		DdcError
	},
	
	Reset
	
};

// Core parameters
static struct {
	uint8_t Enable;
	uint8_t PacketSize;
} CoreParameter = {
	0,
	40
};

// Mode - BaudeRate Table
static float BaudRateTable[] = {
	50.f, // 100 bit/sec
	80.f, // 160 bit/sec
	100.f, // 200 bit/sec
	125.f, // 250 bit/sec
	160.f, // 320 bit/sec
	200.f, // 400 bit/sec
	250.f, // 500 bit/sec
	400.f, // 800 bit/sec
	500.f, // 1000 bit/sec
	1000.f // !!! 400 bit/sec, devsation = 1000 Hz, Fcenter = 0 Hz for ALE
};
static float FrequencyShift[] = {
	456.7f,
	416.7f,
	456.7f,
	456.7f,
	416.7f,
	456.7f,
	456.7f, 
	256.7f,
	456.7f,
	0.f // !!! 400 bit/sec, devsation = 1000 Hz, Fcenter = 0 Hz for ALE
};

// Description master functions
static void ParserInit(void){StoronaDriver.Init();}
static void ParserMode(uint8_t mode){
	if(mode > 9){
		Print("ERROR: Uncorrect mode "); PrintLn(mode);
	} else {
		CoreParameter.Enable = 1;
		DemodulatorQFSK.RelativeFrequencyShift.Set(FrequencyShift[mode] / (float)RECEIVER_BANDWIDTH);
		DemodulatorQFSK.RelativeBaudRate.Set(BaudRateTable[mode] / (float)RECEIVER_BANDWIDTH);
		ModulatorQFSK.RelativeFrequencyShift.Set(FrequencyShift[mode] / (float)TRANSFER_BANDWIDTH);
		ModulatorQFSK.RelativeBaudRate.Set(BaudRateTable[mode] / (float)TRANSFER_BANDWIDTH);
	}
}
static void ParserStop(void){
	CoreParameter.Enable = 0;
	DucDriver.Mode.Set(0);
	DucDriver.Buffer.Clear();
	DdcDriver.Mode.Set(0);

}
static void ParserSizePacket(uint8_t size){
	if(size < 64)
		CoreParameter.PacketSize = size;
}
static void ParserEthernetSet(uint32_t ip, uint32_t mask, uint16_t port){
	ethernet parameters;
	parameters.Ip = ip;
	mask = 0xFFFFFF00; // unused
	parameters.Mask = mask;
	port = 80; // unused
	
	Storona.Stop();
	
	Device.Ethernet.Set(parameters);
}
static void ParserEthernetGet(void){
	if(CoreParameter.Enable == 0){
		ethernet parameters = Device.Ethernet.Get();
		StoronaDriver.Ethernet(parameters.Ip, parameters.Mask, (uint16_t)80);
	}
}
static void ParserBit(uint8_t bit){BitBuffer.Bit.Add(bit);}
static void ParserCombinationModulator(uint8_t combination){ModulatorQFSK.Combination.Set(combination);}
static void ParserCombinationDemodulator(uint8_t combination){DemodulatorQFSK.Combination.Set(combination);}
static void ParserFrequencyDuc(uint32_t frequency){DucDriver.Frequency.Set(frequency);}
static void ParserFrequencyDdc(uint32_t frequency){DdcDriver.Frequency.Set(frequency);}
static void ParserInversionSet(uint8_t inv){DemodulatorQFSK.Inversion.Set(inv);}
static void DucMode(uint32_t mode){mode += 0;}
static void DucWidth(uint32_t width){width += 0;}
static void DucFrequency(uint32_t frequency){frequency += 0;}
static void DucPercent(uint32_t percent){SemplSender.Percent.Set(percent);}
static void DucError(uint32_t error){error += 0;}
static void DdcMode(uint32_t mode){mode += 0;}
static void DdcWidth(uint32_t width){width += 0;}
static void DdcFrequency(uint32_t frequency){frequency += 0;}
static void DdcSempl(complex sempl){DemodulatorQFSK.AddSempl(sempl);}
static void DdcError(uint32_t error){error += 0;}
static void Reset(void){
	CoreParameter.Enable = 0;
	CoreParameter.PacketSize = 40;
}
