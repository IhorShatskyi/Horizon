#include "socket.h"
#include "print.h"
#include "define.h"
#include "spi.h"
#include "delay.h"
#include "wiznet.h"

// WIZNET CS
#define CSS_OFF GPIOD->BSRR=GPIO_BSRR_BR_2;
#define CSS_ON  GPIOD->BSRR=GPIO_BSRR_BS_2;

static void socketRead(uint8_t, list *);
static void socketWrite(uint8_t, list *);
static void socketStateMachine(uint8_t);

const struct socketFunctions Socket = {
	socketRead,
	socketWrite,
	socketStateMachine
};

void socketRead(uint8_t socket, list *src){

	// state
	if(Wiznet.Socket.Status.Get(socket) != SR_SOCK_ESTABLISHED) return;

	// count
	uint32_t count = Wiznet.Socket.RxReceivedSize.Get(socket);
	if(!count) return;		
	
	// start adress
	uint16_t address = Wiznet.Socket.RxReadPointer.Get(socket);

	// reading
	uint8_t *pointer = (uint8_t *)&address + 2;
	Spi.Write(*--pointer);
	Spi.Write(*--pointer);
	Spi.Write((uint8_t)(((socket + 2) << 3) | READ | VDMODE));
	for(uint32_t i = 0; i < count; i++)
		List.Write.Ubyte(src, Spi.Read());
  CSS_ON
  Delay.MicroSecond(1.f);
	CSS_OFF
	
	// modify adress 
	Wiznet.Socket.RxReadPointer.Set(socket, address + (uint16_t)count);

	// send command
	Wiznet.Socket.Command.Set(socket, CR_RECV);
}
void socketWrite(uint8_t socket, list *src){

	// count list
	uint32_t count = List.Count(src);
	if(!count) return;

	// port state
	//if(Wiznet.Socket.Status.Get(socket) != SR_SOCK_ESTABLISHED) {
	//	List.Clear(src);
	//	return;
	//}

	// check free size
	uint16_t freeSize = 0, repeat = 0;
	do{
		repeat = freeSize;
		freeSize = Wiznet.Socket.TxFreeSize.Get(socket);
	}
	while(freeSize != repeat);
	if(!freeSize) return;
	if(freeSize < count) count = freeSize;
		
	// start adress
	uint16_t address = Wiznet.Socket.TxWritePointer.Get(socket);

	// writing
	uint8_t *pointer = (uint8_t *)&address + 2;
	Spi.Write(*--pointer);
	Spi.Write(*--pointer);
	Spi.Write((uint8_t)(((socket + 1) << 3) | WRITE | VDMODE));
	for(uint32_t i = 0; i < count; i++)
		Spi.Write(List.Read.Ubyte(src));
  CSS_ON
  Delay.MicroSecond(1.f);
	CSS_OFF
	
	// modify adress 
	Wiznet.Socket.TxWritePointer.Set(socket, address + (uint16_t)count);

	// send command
	Wiznet.Socket.Command.Set(socket, CR_SEND);
}
void socketStateMachine(uint8_t socket){

	uint8_t state = Wiznet.Socket.Status.Get(socket);
	if (state == SR_SOCK_ESTABLISHED && !Wiznet.Common.PhyConfiguration.LinkStatus.Get())
		Wiznet.Socket.Command.Set(socket, CR_CLOSE);
	else if (state == SR_SOCK_INIT)
		Wiznet.Socket.Command.Set(socket, CR_LISTEN);	
	else if (state == SR_SOCK_CLOSED)
		Wiznet.Socket.Command.Set(socket, CR_OPEN);
	else if (state == SR_SOCK_TIME_WAIT)
		Wiznet.Socket.Command.Set(socket, CR_CLOSE);
	else if (state == SR_SOCK_CLOSE_WAIT)
		Wiznet.Socket.Command.Set(socket, CR_CLOSE);
	
	// Debug
	const char *SocketStatusRegisterString[] = {
		[SR_SOCK_CLOSED] = "Closed",
		[SR_SOCK_INIT] = "Init",
		[SR_SOCK_TIME_WAIT] = "Time wait",	
		[SR_SOCK_CLOSE_WAIT] = "Close wait",
		[SR_SOCK_LISTEN] = "Listen",
		[SR_SOCK_ESTABLISHED] = "Established",
		[SR_SOCK_UDP] = "UDP",
		[SR_SOCK_MACRAW] = "MACRAW",
		[SR_SOCK_SYNSENT] = "SYNSENT",
		[SR_SOCK_SYNRECV] = "SYNRECV",
		[SR_SOCK_FIN_WAIT] = "Fin wait",
		[SR_SOCK_CLOSING] = "Closing",
		[SR_SOCK_LAST_ACK] = "Last ACK",
		[17] = "Config wait"
	};
	static uint8_t oldState[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	uint8_t socketNumber = (socket - 1) >> 2;
	if(oldState[socketNumber] == state) return;
	Print("Port "); Print(Wiznet.Socket.SourcePort.Get(socket));
	Print(" (socket "); Print(socketNumber); Print(") ");
	PrintLn(SocketStatusRegisterString[state]);
	oldState[(socket - 1) >> 2] = state;	
}
