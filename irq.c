#include "irq.h"

#define SIZE_LIST_USB 2048
#define SIZE_LIST_DDC 2048
#define SIZE_LIST_DUC 2048

static uint8_t usb[SIZE_LIST_USB];
static uint8_t ddc[SIZE_LIST_DDC];
static uint8_t duc[SIZE_LIST_DUC];

list listUsbRx = {"List USB RX", usb, SIZE_LIST_USB, 0, 0};
list listDdcRx = {"List DDC RX", ddc, SIZE_LIST_DDC, 0, 0};
list listDucRx = {"List DUC RX", duc, SIZE_LIST_DUC, 0, 0};

void USART1_IRQHandler(void){
	USART1->SR += 0; // <- !!!
	List.Write.Ubyte(&listUsbRx, (uint8_t)(USART1->DR));
}

void USART2_IRQHandler(void){
	USART2->SR += 0; // <- !!!
	List.Write.Ubyte(&listDdcRx, (uint8_t)(USART2->DR));
}

void USART3_IRQHandler(void){
	USART3->SR += 0; // <- !!!
	List.Write.Ubyte(&listDucRx, (uint8_t)(USART3->DR));
}
