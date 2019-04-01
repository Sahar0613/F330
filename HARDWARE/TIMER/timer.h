#ifndef _TIMER_H
#define _TIMER_H

#include "sys.h"

extern u8 loop1msFlag;
extern u8 loop2msFlag;

void TIM3_Int_Init(u16 arr,u16 psc);

#endif
