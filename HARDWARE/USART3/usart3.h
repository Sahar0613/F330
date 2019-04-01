#ifndef _USART3_H
#define _USART3_H

#include "sys.h"

void USART3_Init_Config(void);

extern u8 Flag;
extern unsigned char sbus_rx_buffer[25];

#endif
