#ifndef IRQ_H
#define IRQ_H

#include "stm32f446xx.h"
#include "list.h"

extern list listUsbRx, listDdcRx, listDucRx;

void USART1_IRQHandler(void); // USB
void USART2_IRQHandler(void); // DDC
void USART3_IRQHandler(void); // DUC

#endif
