#ifndef SOCKET_H
#define SOCKET_H

#include "stm32f446xx.h"
#include "list.h"
#include "w5500.h"

extern const struct socketFunctions {
	void (*const Read)(uint8_t, list *);
	void (*const Write)(uint8_t, list *);
	void (*const StateMachine)(uint8_t);
} Socket;

#endif
