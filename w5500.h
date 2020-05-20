#ifndef W5500_H
#define W5500_H

//Control Phase
#define READ 0 // read operation
#define WRITE 4 // write operation
#define VDMODE 0 // VDM mode
#define FDMODE1 1 // FDM mode 1 byte
#define FDMODE2 2 // FDM mode 2 bytes
#define FDMODE4 3 // FDM mode 4 bytes

// Blocks
#define COMMON 0x00 // Common Register
#define SOCKET0 0x01 // Socket 0 Register
#define SOCKET0_TX 0x02 // Socket 0 TX Buffer
#define SOCKET0_RX 0x03 // Socket 0 RX Buffer 
#define SOCKET1 0x05 // Socket 1 Register 
#define SOCKET1_TX 0x06 // Socket 1 TX Buffer
#define SOCKET1_RX 0x07 // Socket 1 RX Buffer
#define SOCKET2 0x09 // Socket 2 Register
#define SOCKET2_TX 0x0A // Socket 2 TX Buffer
#define SOCKET2_RX 0x0B // Socket 2 RX Buffer
#define SOCKET3 0x0D // Socket 3 Register
#define SOCKET3_TX 0x0E // Socket 3 TX Buffer
#define SOCKET3_RX 0x0F // Socket 3 RX Buffer
#define SOCKET4 0x011 // Socket 4 Register
#define SOCKET4_TX 0x012 // Socket 4 TX Buffer
#define SOCKET4_RX 0x013 // Socket 4 RX Buffer
#define SOCKET5 0x015 // Socket 5 Register
#define SOCKET5_TX 0x016 // Socket 5 TX Buffer
#define SOCKET5_RX 0x017 // Socket 5 RX Buffer
#define SOCKET6 0x019 // Socket 6 Register
#define SOCKET6_TX 0x01A // Socket 6 TX Buffer
#define SOCKET6_RX 0x01B // Socket 6 RX Buffer
#define SOCKET7 0x1D // Socket 7 Register
#define SOCKET7_TX 0x1E // Socket 7 TX Buffer
#define SOCKET7_RX 0x1F // Socket 7 RX Buffer

// Common Block
#define MR 0x0000 // Mode
#define GAR0 0x0001 // Gateway Address
#define GAR1 0x0002
#define GAR2 0x0003
#define GAR3 0x0004
#define GAR GAR0
#define SUBR0 0x0005 // Subnet Mask Address
#define SUBR1 0x0006
#define SUBR2 0x0007
#define SUBR3 0x0008
#define SUBR SUBR0
#define SHAR0 0x0009 // Source Hardware Address
#define SHAR1 0x000A
#define SHAR2 0x000B
#define SHAR3 0x000C
#define SHAR4 0x000D
#define SHAR5 0x000E
#define SHAR SHAR0
#define SIPR0 0x000F // Source IP Address
#define SIPR1 0x0010
#define SIPR2 0x0011
#define SIPR3 0x0012
#define SIPR SIPR0
#define INTLEVEL0 0x0013 // Interrupt Low Level Timer
#define INTLEVEL1 0x0014
#define INTLEVEL INTLEVEL0
#define IR 0x0015 // Interrupt
#define IMR 0x0016 // Interrupt Mask 
#define SIR 0x0017 // Socket Interrupt
#define SIMR 0x0018 // Socket Interrupt Mask
#define RTR0 0x0019 // Retry Time
#define RTR1 0x001A
#define RTR RTR0
#define RCR 0x001B // Retry Count 
#define PTIMER 0x001C // PPP LCP Request Timer 
#define PMAGIC 0x001D // PPP LCP Magic number
#define PHAR0 0x001E // PPP Destination MAC Address
#define PHAR1 0x001F
#define PHAR2 0x0020
#define PHAR3 0x0021
#define PHAR4 0x0022
#define PHAR5 0x0023
#define PHAR PHAR0
#define PSID0 0x0024 // PPP Session Identification
#define PSID1 0x0025
#define PSID PSID0
#define PMRU0 0x0026 // PPP Maximum Segment Size
#define PMRU1 0x0027
#define PMRU PMRU0
#define UIPR0 0x0028 // Unreachable IP address
#define UIPR1 0x0029
#define UIPR2 0x002A
#define UIPR3 0x002B
#define UIPR UIPR0
#define UPORTR0 0x002C // Unreachable Port
#define UPORTR1 0x002D
#define UPORTR UPORTR0
#define PHYCFGR 0x002E // PHY Configuration
#define VERSIONR 0x0039// Chip version

// Mode Register
#define MR_RST 7 // Reset
#define MR_WOL 5 // Wake on LAN
#define MR_PB 4 // Ping Block Mode
#define MR_PPPOE 3 // PPPoE Mode
#define MR_FARP 1 // Force ARP

// Interrupt Register
#define IR_CONFLICT 7 // IP Conflict
#define IR_UNREACH 6 // Destination unreachable
#define IR_PPPoE 5 // PPPoE Connection Close
#define IR_MP 4 // Magic Packet

// Interrupt Mask Register
#define IMR_IM_IR7 7 // IP Conflict Interrupt Mask
#define IMR_IM_IR6 6 // Destination unreachable Interrupt Mask
#define IMR_IM_IR5 5 // PPPoE Close Interrupt Mask
#define IMR_IM_IR4 4 // Magic Packet Interrupt Mask

// Socket Interrupt Register
#define SIR_S7_INT 7
#define SIR_S6_INT 6
#define SIR_S5_INT 5
#define SIR_S4_INT 4
#define SIR_S3_INT 3
#define SIR_S2_INT 2
#define SIR_S1_INT 1
#define SIR_S0_INT 0

// Socket Interrupt Mask Register
#define SIMR_S7_IMR 7
#define SIMR_S6_IMR 6
#define SIMR_S5_IMR 5
#define SIMR_S4_IMR 4
#define SIMR_S3_IMR 3
#define SIMR_S2_IMR 2
#define SIMR_S1_IMR 1
#define SIMR_S0_IMR 0

// PHY Configuration Register
#define PHYCFGR_RST 7 // Reset
#define PHYCFGR_OPMD 6 // Configure PHY Operation Mode
#define PHYCFGR_OPMDC 3 // Operation Mode Configuration
#define PHYCFGR_DPX 2 // Duplex Status
#define PHYCFGR_SPD 1 // Speed Status
#define PHYCFGR_LNK 0 // Link Status

// Socket n Block
#define Sn_MR 0x0000 // Mode
#define Sn_CR 0x0001 // Command
#define Sn_IR 0x0002 // Interrupt
#define Sn_SR 0x0003 // Status
#define Sn_PORT0 0x0004 // Source Port
#define Sn_PORT1 0x0005
#define Sn_PORT Sn_PORT0
#define Sn_DHAR0 0x0006 // Destination Hardware Address
#define Sn_DHAR1 0x0007
#define Sn_DHAR2 0x0008
#define Sn_DHAR3 0x0009
#define Sn_DHAR4 0x000A
#define Sn_DHAR5 0x000B
#define Sn_DHAR Sn_DHAR0
#define Sn_DIPR0 0x000C // Destination IP Address
#define Sn_DIPR1 0x000D
#define Sn_DIPR2 0x000E
#define Sn_DIPR3 0x000F
#define Sn_DIPR Sn_DIPR0
#define Sn_DPORT0 0x0010 // Destination Port
#define Sn_DPORT1 0x0011
#define Sn_DPORT Sn_DPORT0
#define Sn_MSSR0 0x0012 // Maximum Segment Size
#define Sn_MSSR1 0x0013
#define Sn_MSSR Sn_MSSR0
#define Sn_TOS 0x0015 // IP TOS
#define Sn_TTL 0x0016 // IP TTL
#define Sn_RXBUF_SIZE 0x001E // Receive Buffer Size
#define Sn_TXBUF_SIZE 0x001F // Transmit Buffer Size
#define Sn_TX_FSR0 0x0020 // TX Free Size
#define Sn_TX_FSR1 0x0021
#define Sn_TX_FSR Sn_TX_FSR0
#define Sn_TX_RD0 0x0022 // TX Read Pointer
#define Sn_TX_RD1 0x0023
#define Sn_TX_RD Sn_TX_RD0
#define Sn_TX_WR0 0x0024 // TX Write Pointer
#define Sn_TX_WR1 0x0025
#define Sn_TX_WR Sn_TX_WR0
#define Sn_RX_RSR0 0x0026 // RX Received Size
#define Sn_RX_RSR1 0x0027
#define Sn_RX_RSR Sn_RX_RSR0
#define Sn_RX_RD0 0x0028 // RX Read Pointer
#define Sn_RX_RD1 0x0029
#define Sn_RX_RD Sn_RX_RD0
#define Sn_RX_WR0 0x002A // RX Write Pointer
#define Sn_RX_WR1 0x002B
#define Sn_RX_WR Sn_RX_WR0
#define Sn_IMR 0x002C // Interrupt Mask
#define Sn_FRAG0 0x002D // Fragment Offset in IP header
#define Sn_FRAG1 0x002E
#define Sn_FRAG Sn_FRAG0
#define Sn_KPALVTR 0x002F // Keep alive timer 

// Mode Register
#define MR_MULTI_MFEN 7 // Multicasting in UDP mode / MAC Filter Enable in MACRAW mode
#define MR_BCASTB 6 // Broadcast Blocking in MACRAW and UDP mode
#define ND_MC_MMB 5 // Use No Delayed ACK / Multicast / Multicast Blocking in MACRAW mode
#define MR_UCASTB_MIP6B 4 // UNICAST Blocking in UDP mode / IPv6 packet Blocking in MACRAW mode
#define MR_P3 3 // Protocol
#define MR_P2 2
#define MR_P1 1
#define MR_P0 0
#define MR_CLOSED 0
#define MR_TCP 1
#define MR_UDP 2
#define MR_MACRAW 4

// Command Register
#define CR_OPEN 0x01
#define CR_LISTEN 0x02
#define CR_CONNECT 0x04
#define CR_DISCON 0x08
#define CR_CLOSE 0x10
#define CR_SEND 0x20
#define CR_SEND_MAC 0x21
#define CR_SEND_KEEP 0x22
#define CR_RECV 0x40

// Interrupt Register
#define IR_SEND_OK 4
#define IR_TIMEOUT 3
#define IR_RECV 2
#define IR_DISCON 1
#define IR_CON 0

// Status Register
#define SR_SOCK_CLOSED 0x00
#define SR_SOCK_INIT 0x13
#define SR_SOCK_LISTEN 0x14
#define SR_SOCK_ESTABLISHED 0x17
#define SR_SOCK_CLOSE_WAIT 0x1C
#define SR_SOCK_UDP 0x22
#define SR_SOCK_MACRAW 0x42
#define SR_SOCK_SYNSENT 0x15
#define SR_SOCK_SYNRECV 0x16
#define SR_SOCK_FIN_WAIT 0x18
#define SR_SOCK_CLOSING 0x1A
#define SR_SOCK_TIME_WAIT 0X1B
#define SR_SOCK_LAST_ACK 0X1D

// Interrupt Mask Register
#define IMR_SENDOK 4
#define IMR_TIMEOUT 3
#define IMR_RECV 2
#define IMR_DISCON 1
#define IMR_CON 0

#endif
