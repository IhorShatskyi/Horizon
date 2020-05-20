#include "wiznet.h"
#include "define.h"
#include "spi.h"
#include "delay.h"

// wiznet common block
static void commonModeSet(uint8_t);
static uint8_t commonModeGet(void);
static void commonGatewayAddressSet(uint32_t);
static uint32_t commonGatewayAddressGet(void);
static void commonSubnetMaskAddressSet(uint32_t);
static uint32_t commonSubnetMaskAddressGet(void);
static void commonSourceHardwareAddressSet(uint64_t);
static uint64_t commonSourceHardwareAddressGet(void);
static void commonSourceIpAddressSet(uint32_t);
static uint32_t commonSourceIpAddressGet(void);
static void commonInterruptLowLevelTimerSet(uint16_t);
static uint16_t commonInterruptLowLevelTimerGet(void);
static void commonInterruptSet(uint8_t);
static uint8_t commonInterruptGet(void);
static void commonInterruptMaskSet(uint8_t);
static uint8_t commonInterruptMaskGet(void);
static void commonSocketInterruptSet(uint8_t);
static uint8_t commonSocketInterruptGet(void);
static void commonSocketInterruptMaskSet(uint8_t);
static uint8_t commonSocketInterruptMaskGet(void);
static void commonRetryTimeSet(uint16_t);
static uint16_t commonRetryTimeGet(void);
static void commonRetryCountSet(uint8_t);
static uint8_t commonRetryCountGet(void);
static void commonPppLcpRequestTimerSet(uint8_t);
static uint8_t commonPppLcpRequestTimerGet(void);
static void commonPppLcpMagicNumberSet(uint8_t);
static uint8_t commonPppLcpMagicNumberGet(void);
static void commonPppDestinationMacAddressSet(uint64_t);
static uint64_t commonPppDestinationMacAddressGet(void);
static void commonPppSessionIdentificationSet(uint16_t);
static uint16_t commonPppSessionIdentificationGet(void);
static void commonPppMaximumSegmentSizeSet(uint16_t);
static uint16_t commonPppMaximumSegmentSizeGet(void);
static uint32_t commonUnreachableIpAddressGet(void);
static uint16_t commonUnreachablePortGet(void);

static void commonPhyConfigurationRstSet(uint8_t);
static uint8_t commonPhyConfigurationRstGet(void);
static void commonPhyConfigurationOpmdSet(uint8_t);
static uint8_t commonPhyConfigurationOpmdGet(void);
static void commonPhyConfigurationOpmdcSet(uint8_t);
static uint8_t commonPhyConfigurationOpmdcGet(void);
static uint8_t commonPhyConfigurationDpxGet(void);
static uint8_t commonPhyConfigurationSpdGet(void);
static uint8_t commonPhyConfigurationLnkGet(void);

static uint8_t commonChipVersionGet(void);

// wiznet socket block
static void socketModeSet(uint8_t, uint8_t);
static uint8_t socketModeGet(uint8_t);
static void socketCommandSet(uint8_t, uint8_t);
static uint8_t socketCommandGet(uint8_t);
static void socketInterruptSet(uint8_t, uint8_t);
static uint8_t socketInterruptGet(uint8_t);
static uint8_t socketStatusGet(uint8_t);
static void socketSourcePortSet(uint8_t, uint16_t);
static uint16_t socketSourcePortGet(uint8_t);
static void socketDestinationHardwareAddressSet(uint8_t, uint64_t);
static uint64_t socketDestinationHardwareAddressGet(uint8_t);
static void socketDestinationIpAddressSet(uint8_t, uint32_t);
static uint32_t socketDestinationIpAddressGet(uint8_t);
static void socketDestinationPortSet(uint8_t, uint16_t);
static uint16_t socketDestinationPortGet(uint8_t);
static void socketMaximumSegmentSizeSet(uint8_t, uint16_t);
static uint16_t socketMaximumSegmentSizeGet(uint8_t);
static void socketIpTosSet(uint8_t, uint8_t);
static uint8_t socketIpTosGet(uint8_t);
static void socketIpTtlSet(uint8_t, uint8_t);
static uint8_t socketIpTtlGet(uint8_t);
static void socketReceiveBufferSizeSet(uint8_t, uint8_t);
static uint8_t socketReceiveBufferSizeGet(uint8_t);
static void socketTransmitBufferSizeSet(uint8_t, uint8_t);
static uint8_t socketTransmitBufferSizeGet(uint8_t);
static uint16_t socketTxFreeSizeGet(uint8_t);
static uint16_t socketTxReadPointerGet(uint8_t);
static void socketTxWritePointerSet(uint8_t, uint16_t);
static uint16_t socketTxWritePointerGet(uint8_t);
static uint16_t socketRxReceivedSizeGet(uint8_t);
static void socketRxReadPointerSet(uint8_t, uint16_t);
static uint16_t socketRxReadPointerGet(uint8_t);
static uint16_t socketRxWritePointerGet(uint8_t);
static void socketInterruptMaskSet(uint8_t, uint8_t);
static uint8_t socketInterruptMaskGet(uint8_t);
static void socketFragmentOffsetInIpHeaderSet(uint8_t, uint16_t);
static uint16_t socketFragmentOffsetInIpHeaderGet(uint8_t);
static void socketKeepAliveTimerSet(uint8_t, uint8_t);
static uint8_t socketKeepAliveTimerGet(uint8_t);

// wiznet low level functions
static void wiznetWriteByte(uint8_t, uint16_t, uint8_t);
static void wiznetWriteShort(uint8_t, uint16_t, uint16_t);
static void wiznetWriteWord(uint8_t, uint16_t, uint32_t);
static uint8_t wiznetReadByte(uint8_t, uint16_t);
static uint16_t wiznetReadShort(uint8_t, uint16_t);
static uint32_t wiznetReadWord(uint8_t, uint16_t);

const struct wiznetFunctions Wiznet = {
	{
		{commonModeSet, commonModeGet},
		{commonGatewayAddressSet, commonGatewayAddressGet},
		{commonSubnetMaskAddressSet, commonSubnetMaskAddressGet},
		{commonSourceHardwareAddressSet, commonSourceHardwareAddressGet},
		{commonSourceIpAddressSet, commonSourceIpAddressGet},
		{commonInterruptLowLevelTimerSet, commonInterruptLowLevelTimerGet},
		{commonInterruptSet, commonInterruptGet},
		{commonInterruptMaskSet, commonInterruptMaskGet},
		{commonSocketInterruptSet, commonSocketInterruptGet},
		{commonSocketInterruptMaskSet, commonSocketInterruptMaskGet},
		{commonRetryTimeSet, commonRetryTimeGet},
		{commonRetryCountSet,	commonRetryCountGet},
		{commonPppLcpRequestTimerSet,	commonPppLcpRequestTimerGet},
		{commonPppLcpMagicNumberSet, commonPppLcpMagicNumberGet},
		{commonPppDestinationMacAddressSet, commonPppDestinationMacAddressGet},
		{commonPppSessionIdentificationSet, commonPppSessionIdentificationGet},
		{commonPppMaximumSegmentSizeSet, commonPppMaximumSegmentSizeGet},
		{commonUnreachableIpAddressGet},
		{commonUnreachablePortGet},
		{
			{commonPhyConfigurationRstSet, commonPhyConfigurationRstGet},
			{commonPhyConfigurationOpmdSet, commonPhyConfigurationOpmdGet},
			{commonPhyConfigurationOpmdcSet, commonPhyConfigurationOpmdcGet},
			{commonPhyConfigurationDpxGet},
			{commonPhyConfigurationSpdGet},
			{commonPhyConfigurationLnkGet}
		},
		{commonChipVersionGet}
	},{
		{socketModeSet, socketModeGet},
		{socketCommandSet, socketCommandGet},
		{socketInterruptSet, socketInterruptGet},
		{socketStatusGet},
		{socketSourcePortSet, socketSourcePortGet},
		{socketDestinationHardwareAddressSet, socketDestinationHardwareAddressGet},
		{socketDestinationIpAddressSet, socketDestinationIpAddressGet},
		{socketDestinationPortSet, socketDestinationPortGet},
		{socketMaximumSegmentSizeSet, socketMaximumSegmentSizeGet},
		{socketIpTosSet, socketIpTosGet},
		{socketIpTtlSet, socketIpTtlGet},
		{socketReceiveBufferSizeSet, socketReceiveBufferSizeGet},
		{socketTransmitBufferSizeSet, socketTransmitBufferSizeGet},
		{socketTxFreeSizeGet},
		{socketTxReadPointerGet},
		{socketTxWritePointerSet, socketTxWritePointerGet},
		{socketRxReceivedSizeGet},
		{socketRxReadPointerSet, socketRxReadPointerGet},
		{socketRxWritePointerGet},
		{socketInterruptMaskSet, socketInterruptMaskGet},
		{socketFragmentOffsetInIpHeaderSet, socketFragmentOffsetInIpHeaderGet},
		{socketKeepAliveTimerSet, socketKeepAliveTimerGet}
	}
};

static void wiznetWriteByte(uint8_t BSB, uint16_t address, uint8_t src){
	uint8_t *pointer = (uint8_t *)&address + 2;
	Spi.Write(*--pointer);
	Spi.Write(*--pointer);
	Spi.Write((uint8_t)((BSB << 3) | WRITE | FDMODE1));
	Spi.Write(src);
}
static void wiznetWriteShort(uint8_t BSB, uint16_t address, uint16_t src){
	uint8_t *pointer = (uint8_t *)&address + 2;
	Spi.Write(*--pointer);
	Spi.Write(*--pointer);
	Spi.Write((uint8_t)((BSB << 3) | WRITE | FDMODE2));
	pointer = (uint8_t *)&src + 2;
	Spi.Write(*--pointer);
	Spi.Write(*--pointer);
}
static void wiznetWriteWord(uint8_t BSB, uint16_t address, uint32_t src){
	uint8_t *pointer = (uint8_t *)&address + 2;
	Spi.Write(*--pointer);
	Spi.Write(*--pointer);
	Spi.Write((uint8_t)((BSB << 3) | WRITE | FDMODE4));
	pointer = (uint8_t *)&src + 4;
	Spi.Write(*--pointer);
	Spi.Write(*--pointer);
	Spi.Write(*--pointer);
	Spi.Write(*--pointer);
}

static uint8_t wiznetReadByte(uint8_t BSB, uint16_t address){
	uint8_t *pointer = (uint8_t *)&address + 2;
	Spi.Write(*--pointer);
	Spi.Write(*--pointer);
	Spi.Write((uint8_t)((BSB << 3) | READ | FDMODE1));
  return Spi.Read();
}
static uint16_t wiznetReadShort(uint8_t BSB, uint16_t address){
	static uint16_t data = 0;	
	uint8_t *pointer = (uint8_t *)&address + 2;
	Spi.Write(*--pointer);
	Spi.Write(*--pointer);
	Spi.Write((uint8_t)((BSB << 3) | READ | FDMODE2));
	pointer = (uint8_t *)&data + 2;
	*--pointer = Spi.Read();
	*--pointer = Spi.Read();
  return data;
}
static uint32_t wiznetReadWord(uint8_t BSB, uint16_t address){
	static uint32_t data = 0;	
	uint8_t *pointer = (uint8_t *)&address + 2;
	Spi.Write(*--pointer);
	Spi.Write(*--pointer);
	Spi.Write((uint8_t)((BSB << 3) | READ | FDMODE4));
	pointer = (uint8_t *)&data + 4;
	*--pointer = Spi.Read();
	*--pointer = Spi.Read();
	*--pointer = Spi.Read();
	*--pointer = Spi.Read();
  return data;
}

const static struct{
	
	struct wiznetWriteFunctions {
		void (*const Byte)(uint8_t, uint16_t, uint8_t);
		void (*const Short)(uint8_t, uint16_t, uint16_t);
		void (*const Word)(uint8_t, uint16_t, uint32_t);
	} Write;

	struct wiznetReadFunctions {
		uint8_t (*const Byte)(uint8_t, uint16_t address);
		uint16_t (*const Short)(uint8_t, uint16_t address);
		uint32_t (*const Word)(uint8_t, uint16_t address);
	} Read;
	
} WiznetRxTx = {
	
	{
		wiznetWriteByte,
		wiznetWriteShort,
		wiznetWriteWord
	},
	{
		wiznetReadByte,
		wiznetReadShort,
		wiznetReadWord
	}
};

static void commonModeSet(uint8_t src){
	WiznetRxTx.Write.Byte(COMMON, MR, src);
}
static uint8_t commonModeGet(void){
	return WiznetRxTx.Read.Byte(COMMON, MR);
}
static void commonGatewayAddressSet(uint32_t src){
	WiznetRxTx.Write.Word(COMMON, GAR, src);
}
static uint32_t commonGatewayAddressGet(void){
	return WiznetRxTx.Read.Word(COMMON, GAR);
}
static void commonSubnetMaskAddressSet(uint32_t src){
	WiznetRxTx.Write.Word(COMMON, SUBR, src);
}
static uint32_t commonSubnetMaskAddressGet(void){
	return WiznetRxTx.Read.Word(COMMON, SUBR);
}
static void commonSourceHardwareAddressSet(uint64_t src){
	uint8_t *pointer = (uint8_t *)&src + 6;
	WiznetRxTx.Write.Byte(COMMON, SHAR0, *--pointer);
	WiznetRxTx.Write.Byte(COMMON, SHAR1, *--pointer);
	WiznetRxTx.Write.Byte(COMMON, SHAR2, *--pointer);
	WiznetRxTx.Write.Byte(COMMON, SHAR3, *--pointer);
	WiznetRxTx.Write.Byte(COMMON, SHAR4, *--pointer);
	WiznetRxTx.Write.Byte(COMMON, SHAR5, *--pointer);
}
static uint64_t commonSourceHardwareAddressGet(void){
	static uint64_t dst = 0;
	uint8_t *pointer = (uint8_t *)&dst + 6;
	*--pointer = WiznetRxTx.Read.Byte(COMMON, SHAR0);
	*--pointer = WiznetRxTx.Read.Byte(COMMON, SHAR1);
	*--pointer = WiznetRxTx.Read.Byte(COMMON, SHAR2);
	*--pointer = WiznetRxTx.Read.Byte(COMMON, SHAR3);
	*--pointer = WiznetRxTx.Read.Byte(COMMON, SHAR4);
	*--pointer = WiznetRxTx.Read.Byte(COMMON, SHAR5);
	return dst;
}
static void commonSourceIpAddressSet(uint32_t src){
	WiznetRxTx.Write.Word(COMMON, SIPR, src);
}
static uint32_t commonSourceIpAddressGet(void){
	return WiznetRxTx.Read.Word(COMMON, SIPR);
}
static void commonInterruptLowLevelTimerSet(uint16_t src){
	WiznetRxTx.Write.Short(COMMON, INTLEVEL, src);
}
static uint16_t commonInterruptLowLevelTimerGet(void){
	return WiznetRxTx.Read.Short(COMMON, INTLEVEL);
}
static void commonInterruptSet(uint8_t src){
	WiznetRxTx.Write.Byte(COMMON, IR, src);
}
static uint8_t commonInterruptGet(void){
	return WiznetRxTx.Read.Byte(COMMON, IR);
}
static void commonInterruptMaskSet(uint8_t src){
	WiznetRxTx.Write.Byte(COMMON, IMR, src);
}
static uint8_t commonInterruptMaskGet(void){
	return WiznetRxTx.Read.Byte(COMMON, IMR);
}
static void commonSocketInterruptSet(uint8_t src){
	WiznetRxTx.Write.Byte(COMMON, SIR, src);
}
static uint8_t commonSocketInterruptGet(void){
	return WiznetRxTx.Read.Byte(COMMON, SIR);
}
static void commonSocketInterruptMaskSet(uint8_t src){
	WiznetRxTx.Write.Byte(COMMON, SIMR, src);
}
static uint8_t commonSocketInterruptMaskGet(void){
	return WiznetRxTx.Read.Byte(COMMON, SIMR);
}
static void commonRetryTimeSet(uint16_t src){
	WiznetRxTx.Write.Short(COMMON, RTR, src);
}
static uint16_t commonRetryTimeGet(void){
	return WiznetRxTx.Read.Short(COMMON, RTR);
}
static void commonRetryCountSet(uint8_t src){
	WiznetRxTx.Write.Byte(COMMON, RCR, src);
}
static uint8_t commonRetryCountGet(void){
	return WiznetRxTx.Read.Byte(COMMON, RCR);
}
static void commonPppLcpRequestTimerSet(uint8_t src){
	WiznetRxTx.Write.Byte(COMMON, PTIMER, src);
}
static uint8_t commonPppLcpRequestTimerGet(void){
	return WiznetRxTx.Read.Byte(COMMON, PTIMER);
}
static void commonPppLcpMagicNumberSet(uint8_t src){
	WiznetRxTx.Write.Byte(COMMON, PMAGIC, src);
}
static uint8_t commonPppLcpMagicNumberGet(void){
	return WiznetRxTx.Read.Byte(COMMON, PMAGIC);
}
static void commonPppDestinationMacAddressSet(uint64_t src){
	uint8_t *pointer = (uint8_t *)&src + 6;
	WiznetRxTx.Write.Byte(COMMON, PHAR0, *--pointer);
	WiznetRxTx.Write.Byte(COMMON, PHAR1, *--pointer);
	WiznetRxTx.Write.Byte(COMMON, PHAR2, *--pointer);
	WiznetRxTx.Write.Byte(COMMON, PHAR3, *--pointer);
	WiznetRxTx.Write.Byte(COMMON, PHAR4, *--pointer);
	WiznetRxTx.Write.Byte(COMMON, PHAR5, *--pointer);	
}
static uint64_t commonPppDestinationMacAddressGet(void){
	static uint64_t dst = 0;
	uint8_t *pointer = (uint8_t *)&dst + 6;
	*--pointer = WiznetRxTx.Read.Byte(COMMON, PHAR0);
	*--pointer = WiznetRxTx.Read.Byte(COMMON, PHAR1);
	*--pointer = WiznetRxTx.Read.Byte(COMMON, PHAR2);
	*--pointer = WiznetRxTx.Read.Byte(COMMON, PHAR3);
	*--pointer = WiznetRxTx.Read.Byte(COMMON, PHAR4);
	*--pointer = WiznetRxTx.Read.Byte(COMMON, PHAR5);
	return dst;
}
static void commonPppSessionIdentificationSet(uint16_t src){
	WiznetRxTx.Write.Short(COMMON, PSID, src);
}
static uint16_t commonPppSessionIdentificationGet(void){
	return WiznetRxTx.Read.Short(COMMON, PSID);
}
static void commonPppMaximumSegmentSizeSet(uint16_t src){
	WiznetRxTx.Write.Short(COMMON, PMRU, src);
}
static uint16_t commonPppMaximumSegmentSizeGet(void){
	return WiznetRxTx.Read.Short(COMMON, PMRU);
}
static uint32_t commonUnreachableIpAddressGet(void){
	return WiznetRxTx.Read.Word(COMMON, UIPR);
}
static uint16_t commonUnreachablePortGet(void){
	return WiznetRxTx.Read.Short(COMMON, UPORTR);
}
static void commonPhyConfigurationRstSet(uint8_t src){
	WiznetRxTx.Write.Byte(COMMON, PHYCFGR, (uint8_t)(src << PHYCFGR_RST));
}
static uint8_t commonPhyConfigurationRstGet(void){
	return (WiznetRxTx.Read.Byte(COMMON, PHYCFGR) & (1 << PHYCFGR_RST)) >> PHYCFGR_RST;
}
static void commonPhyConfigurationOpmdSet(uint8_t src){
	WiznetRxTx.Write.Byte(COMMON, PHYCFGR, (uint8_t)(src << PHYCFGR_OPMD));
}
static uint8_t commonPhyConfigurationOpmdGet(void){
	return (WiznetRxTx.Read.Byte(COMMON, PHYCFGR) & (1 << PHYCFGR_OPMD)) >> PHYCFGR_OPMD;
}
static void commonPhyConfigurationOpmdcSet(uint8_t src){
	WiznetRxTx.Write.Byte(COMMON, PHYCFGR, (uint8_t)(src << PHYCFGR_OPMDC));
}
static uint8_t commonPhyConfigurationOpmdcGet(void){
	return (WiznetRxTx.Read.Byte(COMMON, PHYCFGR) & (7 << PHYCFGR_OPMDC)) >> PHYCFGR_OPMDC;
}
static uint8_t commonPhyConfigurationDpxGet(void){
	return (WiznetRxTx.Read.Byte(COMMON, PHYCFGR) & (1 << PHYCFGR_DPX)) >> PHYCFGR_DPX;
}
static uint8_t commonPhyConfigurationSpdGet(void){
	return (WiznetRxTx.Read.Byte(COMMON, PHYCFGR) & (1 << PHYCFGR_SPD)) >> PHYCFGR_SPD;
}
static uint8_t commonPhyConfigurationLnkGet(void){
	return (WiznetRxTx.Read.Byte(COMMON, PHYCFGR) & (1 << PHYCFGR_LNK)) >> PHYCFGR_LNK;
}
static uint8_t commonChipVersionGet(void){
	return WiznetRxTx.Read.Byte(COMMON, VERSIONR);
}
static void socketModeSet(uint8_t socket, uint8_t src){
	WiznetRxTx.Write.Byte(socket, Sn_MR, src);
}
static uint8_t socketModeGet(uint8_t socket){
	return WiznetRxTx.Read.Byte(socket, Sn_MR);
}
static void socketCommandSet(uint8_t socket, uint8_t src){
	WiznetRxTx.Write.Byte(socket, Sn_CR, src);
}
static uint8_t socketCommandGet(uint8_t socket){
	return WiznetRxTx.Read.Byte(socket, Sn_CR);
}
static void socketInterruptSet(uint8_t socket, uint8_t src){
	WiznetRxTx.Write.Byte(socket, Sn_IR, src);
}
static uint8_t socketInterruptGet(uint8_t socket){
	return WiznetRxTx.Read.Byte(socket, Sn_IR);
}
static uint8_t socketStatusGet(uint8_t socket){
	return WiznetRxTx.Read.Byte(socket, Sn_SR);
}
static void socketSourcePortSet(uint8_t socket, uint16_t src){
	WiznetRxTx.Write.Short(socket, Sn_PORT, src);
}
static uint16_t socketSourcePortGet(uint8_t socket){
	return WiznetRxTx.Read.Short(socket, Sn_PORT);
}
static void socketDestinationHardwareAddressSet(uint8_t socket, uint64_t src){
	uint8_t *pointer = (uint8_t *)&src + 6;
	WiznetRxTx.Write.Byte(socket, Sn_DHAR0, *--pointer);
	WiznetRxTx.Write.Byte(socket, Sn_DHAR1, *--pointer);
	WiznetRxTx.Write.Byte(socket, Sn_DHAR2, *--pointer);
	WiznetRxTx.Write.Byte(socket, Sn_DHAR3, *--pointer);
	WiznetRxTx.Write.Byte(socket, Sn_DHAR4, *--pointer);
	WiznetRxTx.Write.Byte(socket, Sn_DHAR5, *--pointer);
}
static uint64_t socketDestinationHardwareAddressGet(uint8_t socket){
	static uint64_t dst = 0;
	uint8_t *pointer = (uint8_t *)&dst + 6;
	*--pointer = WiznetRxTx.Read.Byte(socket, Sn_DHAR0);
	*--pointer = WiznetRxTx.Read.Byte(socket, Sn_DHAR1);
	*--pointer = WiznetRxTx.Read.Byte(socket, Sn_DHAR2);
	*--pointer = WiznetRxTx.Read.Byte(socket, Sn_DHAR3);
	*--pointer = WiznetRxTx.Read.Byte(socket, Sn_DHAR4);
	*--pointer = WiznetRxTx.Read.Byte(socket, Sn_DHAR5);
	return dst;
}
static void socketDestinationIpAddressSet(uint8_t socket, uint32_t src){
	WiznetRxTx.Write.Word(socket, Sn_DIPR, src);
}
static uint32_t socketDestinationIpAddressGet(uint8_t socket){
	return WiznetRxTx.Read.Word(socket, Sn_DIPR);
}
static void socketDestinationPortSet(uint8_t socket, uint16_t src){
	WiznetRxTx.Write.Short(socket, Sn_DPORT, src);
}
static uint16_t socketDestinationPortGet(uint8_t socket){
	return WiznetRxTx.Read.Short(socket, Sn_DPORT);
}
static void socketMaximumSegmentSizeSet(uint8_t socket, uint16_t src){
	WiznetRxTx.Write.Short(socket, Sn_MSSR, src);
}
static uint16_t socketMaximumSegmentSizeGet(uint8_t socket){
	return WiznetRxTx.Read.Short(socket, Sn_MSSR);
}
static void socketIpTosSet(uint8_t socket, uint8_t src){
	WiznetRxTx.Write.Byte(socket, Sn_TOS, src);
}
static uint8_t socketIpTosGet(uint8_t socket){
	return WiznetRxTx.Read.Byte(socket, Sn_TOS);
}
static void socketIpTtlSet(uint8_t socket, uint8_t src){
	WiznetRxTx.Write.Byte(socket, Sn_TTL, src);
}
static uint8_t socketIpTtlGet(uint8_t socket){
	return WiznetRxTx.Read.Byte(socket, Sn_TTL);
}
static void socketReceiveBufferSizeSet(uint8_t socket, uint8_t src){
	WiznetRxTx.Write.Byte(socket, Sn_RXBUF_SIZE, src);
}
static uint8_t socketReceiveBufferSizeGet(uint8_t socket){
	return WiznetRxTx.Read.Byte(socket, Sn_RXBUF_SIZE);
}
static void socketTransmitBufferSizeSet(uint8_t socket, uint8_t src){
	WiznetRxTx.Write.Byte(socket, Sn_TXBUF_SIZE, src);
}
static uint8_t socketTransmitBufferSizeGet(uint8_t socket){
	return WiznetRxTx.Read.Byte(socket, Sn_TXBUF_SIZE);
}
static uint16_t socketTxFreeSizeGet(uint8_t socket){
	return WiznetRxTx.Read.Short(socket, Sn_TX_FSR);
}
static uint16_t socketTxReadPointerGet(uint8_t socket){
	return WiznetRxTx.Read.Short(socket, Sn_TX_RD);
}
static void socketTxWritePointerSet(uint8_t socket, uint16_t src){
	WiznetRxTx.Write.Short(socket, Sn_TX_WR, src);
}
static uint16_t socketTxWritePointerGet(uint8_t socket){
	return WiznetRxTx.Read.Short(socket, Sn_TX_WR);
}
static uint16_t socketRxReceivedSizeGet(uint8_t socket){
	return WiznetRxTx.Read.Short(socket, Sn_RX_RSR);
}
static void socketRxReadPointerSet(uint8_t socket, uint16_t src){
	WiznetRxTx.Write.Short(socket, Sn_RX_RD, src);
}
static uint16_t socketRxReadPointerGet(uint8_t socket){
	return WiznetRxTx.Read.Short(socket, Sn_RX_RD);
}
static uint16_t socketRxWritePointerGet(uint8_t socket){
	return WiznetRxTx.Read.Short(socket, Sn_RX_WR);
}
static void socketInterruptMaskSet(uint8_t socket, uint8_t src){
	WiznetRxTx.Write.Byte(socket, Sn_IMR, src);
}
static uint8_t socketInterruptMaskGet(uint8_t socket){
	return WiznetRxTx.Read.Byte(socket, Sn_IMR);
}
static void socketFragmentOffsetInIpHeaderSet(uint8_t socket, uint16_t src){
	WiznetRxTx.Write.Short(socket, Sn_FRAG, src);
}
static uint16_t socketFragmentOffsetInIpHeaderGet(uint8_t socket){
	return WiznetRxTx.Read.Short(socket, Sn_FRAG);
}
static void socketKeepAliveTimerSet(uint8_t socket, uint8_t src){
	WiznetRxTx.Write.Byte(socket, Sn_KPALVTR, src);
}
static uint8_t socketKeepAliveTimerGet(uint8_t socket){
	return WiznetRxTx.Read.Byte(socket, Sn_KPALVTR);
}
