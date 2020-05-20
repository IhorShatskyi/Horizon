#include "horizonCore.h"
#include "horizonDriver.h"
#include "ducDriver.h"
#include "ddcDriver.h"
#include "device.h"

// Master functions
static void DdcMode(uint32_t);
static void DdcWidth(uint32_t);
static void DdcFrequency(uint32_t);
static void DdcSempl(complex);
static void DdcError(uint32_t);
static void DucMode(uint32_t);
static void DucWidth(uint32_t);
static void DucFrequency(uint32_t);
static void DucPercent(uint32_t);
static void DucError(uint32_t);
static void ParserDdcModeSet(uint32_t);
static void ParserDdcModeGet(void);				
static void ParserDdcWidthSet(uint32_t);
static void ParserDdcWidthGet(void);				
static void ParserDdcFrequencySet(uint32_t);
static void ParserDdcFrequencyGet(void);				
static void ParserDdcReset(void);
static void ParserDucModeSet(uint32_t);
static void ParserDucModeGet(void);				
static void ParserDucWidthSet(uint32_t);
static void ParserDucWidthGet(void);				
static void ParserDucFrequencySet(uint32_t);
static void ParserDucFrequencyGet(void);				
static void ParserDucReset(void);
static void ParserDucBufferPercent(void);	
static void ParserDucBufferClear(void);
static void ParserDucBufferWrite(complex);				
static void ParserEthernetSet(uint32_t, uint32_t, uint32_t, uint32_t);
static void ParserEthernetGet(void);
static void ParserEthernetReset(void);
static void ParserInit(void);

// Master struct
const struct horizonCoreFunctions HorizonCore = {

	{
		DdcMode,
		DdcWidth,
		DdcFrequency,
		DdcSempl,
		DdcError
	},
	{
		DucMode,
		DucWidth,
		DucFrequency,
		DucPercent,
		DucError
	},
	{
		{
			{
				ParserDdcModeSet,
				ParserDdcModeGet				
			},
			{
				ParserDdcWidthSet,
				ParserDdcWidthGet			
			},
			{
				ParserDdcFrequencySet,
				ParserDdcFrequencyGet			
			},
			ParserDdcReset
		},
		{
			{
				ParserDucModeSet,
				ParserDucModeGet
			},
			{
				ParserDucWidthSet,
				ParserDucWidthGet
			},
			{
				ParserDucFrequencySet,
				ParserDucFrequencyGet
			},
			ParserDucReset,
			{
				ParserDucBufferPercent,
				ParserDucBufferClear,
				ParserDucBufferWrite
			}
		},
		{
			ParserEthernetSet,
			ParserEthernetGet,
			ParserEthernetReset
		},
		ParserInit
	}
};

// Description master functions
static void DdcMode(uint32_t mode){HorizonDriver.Ddc.Mode(mode);}
static void DdcWidth(uint32_t width){HorizonDriver.Ddc.Width(width);}
static void DdcFrequency(uint32_t frequency){HorizonDriver.Ddc.Frequency(frequency);}
static void DdcSempl(complex sempl){HorizonDriver.Ddc.Sempl(sempl);}
static void DdcError(uint32_t error){HorizonDriver.Error(error);}
static void DucMode(uint32_t mode){HorizonDriver.Duc.Mode(mode);}
static void DucWidth(uint32_t width){HorizonDriver.Duc.Width(width);}
static void DucFrequency(uint32_t frequency){HorizonDriver.Duc.Frequency(frequency);}
static void DucPercent(uint32_t percent){HorizonDriver.Duc.Percent(percent);}
static void DucError(uint32_t error){HorizonDriver.Error(error);}
static void ParserDdcModeSet(uint32_t mode){DdcDriver.Mode.Set(mode);}
static void ParserDdcModeGet(void){DdcDriver.Mode.Get();}
static void ParserDdcWidthSet(uint32_t width){DdcDriver.Width.Set(width);}
static void ParserDdcWidthGet(void){DdcDriver.Width.Get();}
static void ParserDdcFrequencySet(uint32_t frequency){DdcDriver.Frequency.Set(frequency);}
static void ParserDdcFrequencyGet(void){DdcDriver.Frequency.Get();}
static void ParserDdcReset(void){DdcDriver.Reset();}
static void ParserDucModeSet(uint32_t mode){DucDriver.Mode.Set(mode);}
static void ParserDucModeGet(void){DucDriver.Mode.Get();}
static void ParserDucWidthSet(uint32_t width){DucDriver.Width.Set(width);}
static void ParserDucWidthGet(void){DucDriver.Width.Get();}
static void ParserDucFrequencySet(uint32_t frequency){DucDriver.Frequency.Set(frequency);}
static void ParserDucFrequencyGet(void){DucDriver.Frequency.Get();}
static void ParserDucReset(void){DucDriver.Reset();}
static void ParserDucBufferPercent(void){DucDriver.Buffer.Percent();}
static void ParserDucBufferClear(void){DucDriver.Buffer.Clear();}
static void ParserDucBufferWrite(complex sempl){DucDriver.Buffer.Write(sempl);}
static void ParserEthernetSet(uint32_t ip, uint32_t mask, uint32_t port, uint32_t gateWay){
	ethernet parameters;
	parameters.Ip = ip;
	parameters.Mask = mask;
	port += 0; // not used
	gateWay += 0; // not used
	Device.Ethernet.Set(parameters);
}
static void ParserEthernetGet(void){
	uint32_t port = 81, gateWay = 0xC0A80000;
	ethernet parameters = Device.Ethernet.Get();
	HorizonDriver.Ethernet(parameters.Ip, parameters.Mask, port, gateWay);
}
static void ParserEthernetReset(void){Device.Ethernet.Reset();}
static void ParserInit(void){HorizonDriver.Init(Device.Init());}
