#include "stm32f446xx.h"
#include "init.h"
#include "print.h"
#include "welcome.h"
#include "dmaTransfer.h"
#include "list.h"
#include "button.h"
#include "delay.h"
#include "led.h"
#include "socket.h"
#include "wiznet.h"
#include "irq.h"
#include "ducDriver.h"
#include "ducParser.h"
#include "ddcDriver.h"
#include "ddcParser.h"
#include "device.h"
#include "horizon.h"
#include "storona.h"
#include "statusBar.h"
#include "debugSempl.h"

int main(void) {
	
	// Test line
	
	// Welcome
	Print(Welcome);

	// Init Status Bar
	DdcParser.Event.StatusBar.Mode = StatusBar.Ddc.Mode;
	DdcParser.Event.StatusBar.Width = StatusBar.Ddc.Width;
	DdcParser.Event.StatusBar.Frequency = StatusBar.Ddc.Frequency;
	DucParser.Event.StatusBar.Mode = StatusBar.Duc.Mode;
	DucParser.Event.StatusBar.Width = StatusBar.Duc.Width;
	DucParser.Event.StatusBar.Frequency = StatusBar.Duc.Frequency;
	DucParser.Event.StatusBar.Percent = StatusBar.Duc.Percent;
	
	// Init peripherals
	Init.Gpio();
	Init.Systick();
	Init.Tim();
	Init.Spi();
	Init.Uart();
	Init.Dma();
	Init.FlashMemory();
	Init.Rcc();
	Init.W5500();
	Init.Irq();
	
	// Reload timer
  TIM2->ARR = 0xffffffff;

	uint32_t status;
	static uint32_t oldStatusSocket0 = SR_SOCK_CLOSED;
	static uint32_t oldStatusSocket1 = SR_SOCK_CLOSED;

	for(;;) {

		// Horizon
		status = Wiznet.Socket.Status.Get(SOCKET0);
		if(oldStatusSocket0 != status){
			if(status == SR_SOCK_ESTABLISHED){
				Wiznet.Socket.Command.Set(SOCKET1, CR_CLOSE);
				oldStatusSocket1 = SR_SOCK_CLOSED;
				Storona.Stop();
				Horizon.Init();
			}
			if(oldStatusSocket0 == SR_SOCK_ESTABLISHED) Horizon.Stop();
			oldStatusSocket0 = status;
		}	
		if(status == SR_SOCK_ESTABLISHED)
			Horizon.Run();

		// Storona
		status = Wiznet.Socket.Status.Get(SOCKET1);	
		if(oldStatusSocket1 != status){
			if(status == SR_SOCK_ESTABLISHED){
				Wiznet.Socket.Command.Set(SOCKET0, CR_CLOSE);
				oldStatusSocket0 = SR_SOCK_CLOSED;
				Horizon.Stop();
				Storona.Init();
			}
			if(oldStatusSocket1 == SR_SOCK_ESTABLISHED) Storona.Stop();
			oldStatusSocket1 = status;			
			
		}	
		if(status == SR_SOCK_ESTABLISHED)
			Storona.Run();		

		// Socket StateMachine
		Socket.StateMachine(SOCKET0);
		Socket.StateMachine(SOCKET1);
		Socket.StateMachine(SOCKET7);

		// Send to DDC and DUC
		DmaTransfer.Uart2(&listDdcDriver);	
		DmaTransfer.Uart3(&listDucDriver);		

		// Parsers DDC and DUC
		DdcParser.Parse(&listDdcRx);
		DucParser.Parse(&listDucRx);
		
		// LED
		Led.Update();
		
		// Buttom
		Button.Check();
		
		// StatusBar
		StatusBar.Show();		
		
		// Print out
		DmaTransfer.Uart1(&listPrint);
		
		// Debug Sempl
		if(List.Count(&Sempls) >= 512)
			Socket.Write(SOCKET7, &Sempls);
		
	}
	
}
