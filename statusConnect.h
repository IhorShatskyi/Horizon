#ifndef STATUS_CONNECT_H
#define STATUS_CONNECT_H

#include "stm32f446xx.h"

enum status {
	DISCONNECT = 0,
	CONNECT = 1
};

extern const struct statusConnectFunctions {
	enum status (*const Eth)(void);
	enum status (*const Usb)(void);
} StatusConnect;

#endif
