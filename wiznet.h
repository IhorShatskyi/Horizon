#ifndef WIZNET_H
#define WIZNET_H

#include "stm32f446xx.h"
#include "w5500.h"

extern const struct wiznetFunctions {

	struct commonBlock {
		struct {void (*const Set)(uint8_t); uint8_t (*const Get)(void);} Mode;
		struct {void (*const Set)(uint32_t); uint32_t (*const Get)(void);} GatewayAddress;
		struct {void (*const Set)(uint32_t); uint32_t (*const Get)(void);} SubnetMaskAddress;
		struct {void (*const Set)(uint64_t); uint64_t (*const Get)(void);} SourceHardwareAddress;		
		struct {void (*const Set)(uint32_t); uint32_t (*const Get)(void);} SourceIpAddress;
		struct {void (*const Set)(uint16_t); uint16_t (*const Get)(void);} InterruptLowLevelTimer;
		struct {void (*const Set)(uint8_t); uint8_t (*const Get)(void);} Interrupt;
		struct {void (*const Set)(uint8_t); uint8_t (*const Get)(void);} InterruptMask;		
		struct {void (*const Set)(uint8_t); uint8_t (*const Get)(void);} SocketInterrupt;
		struct {void (*const Set)(uint8_t); uint8_t (*const Get)(void);} SocketInterruptMask;
		struct {void (*const Set)(uint16_t); uint16_t (*const Get)(void);} RetryTime;
		struct {void (*const Set)(uint8_t); uint8_t (*const Get)(void);} RetryCount;		
		struct {void (*const Set)(uint8_t);	uint8_t (*const Get)(void);} PppLcpRequestTimer;		
		struct {void (*const Set)(uint8_t);	uint8_t (*const Get)(void);} PppLcpMagicNumber;			
		struct {void (*const Set)(uint64_t); uint64_t (*const Get)(void);} PppDestinationMacAddress;	
		struct {void (*const Set)(uint16_t); uint16_t (*const Get)(void);} PppSessionIdentification;
		struct {void (*const Set)(uint16_t); uint16_t (*const Get)(void);} PppMaximumSegmentSize;		
		struct {uint32_t (*const Get)(void);} UnreachableIpAddress;
		struct {uint16_t (*const Get)(void);} UnreachablePort;
		struct {
			struct{void (*const Set)(uint8_t); uint8_t (*const Get)(void);} Reset;
			struct{void (*const Set)(uint8_t); uint8_t (*const Get)(void);} ConfigurePhyOperationMode;
			struct{void (*const Set)(uint8_t); uint8_t (*const Get)(void);} OperationModeConfigurationBit;
			struct{uint8_t (*const Get)(void);} DuplexStatus;			
			struct{uint8_t (*const Get)(void);} SpeedStatus;
			struct{uint8_t (*const Get)(void);} LinkStatus;
		} PhyConfiguration;
		struct {uint8_t (*const Get)(void);} ChipVersion;
	} Common;

	struct socketBlock {
		struct {void (*const Set)(uint8_t, uint8_t); uint8_t (*const Get)(uint8_t);} Mode;
		struct {void (*const Set)(uint8_t, uint8_t); uint8_t (*const Get)(uint8_t);} Command;
		struct {void (*const Set)(uint8_t, uint8_t); uint8_t (*const Get)(uint8_t);} Interrupt;
		struct {uint8_t (*const Get)(uint8_t);} Status;
		struct {void (*const Set)(uint8_t, uint16_t);	uint16_t (*const Get)(uint8_t);} SourcePort;
		struct {void (*const Set)(uint8_t, uint64_t);	uint64_t (*const Get)(uint8_t);} DestinationHardwareAddress;
		struct {void (*const Set)(uint8_t, uint32_t);	uint32_t (*const Get)(uint8_t);} DestinationIpAddress;
		struct {void (*const Set)(uint8_t, uint16_t);	uint16_t (*const Get)(uint8_t);} DestinationPort;
		struct {void (*const Set)(uint8_t, uint16_t);	uint16_t (*const Get)(uint8_t);} MaximumSegmentSize;
		struct {void (*const Set)(uint8_t, uint8_t); uint8_t (*const Get)(uint8_t);} IpTos;
		struct {void (*const Set)(uint8_t, uint8_t); uint8_t (*const Get)(uint8_t);} IpTtl;
		struct {void (*const Set)(uint8_t, uint8_t); uint8_t (*const Get)(uint8_t);} ReceiveBufferSize;
		struct {void (*const Set)(uint8_t, uint8_t); uint8_t (*const Get)(uint8_t);} TransmitBufferSize;
		struct {uint16_t (*const Get)(uint8_t);} TxFreeSize;
		struct {uint16_t (*const Get)(uint8_t);} TxReadPointer;
		struct {void (*const Set)(uint8_t, uint16_t);	uint16_t (*const Get)(uint8_t);} TxWritePointer;
		struct {uint16_t (*const Get)(uint8_t);} RxReceivedSize;
		struct {void (*const Set)(uint8_t, uint16_t);	uint16_t (*const Get)(uint8_t);} RxReadPointer;
		struct {uint16_t (*const Get)(uint8_t);} RxWritePointer;
		struct {void (*const Set)(uint8_t, uint8_t); uint8_t (*const Get)(uint8_t);} InterruptMask;
		struct {void (*const Set)(uint8_t, uint16_t);	uint16_t (*const Get)(uint8_t);} FragmentOffsetInIpHeader;
		struct {void (*const Set)(uint8_t, uint8_t); uint8_t (*const Get)(uint8_t);} KeepAliveTimer;
	} Socket;
} Wiznet;

#endif
